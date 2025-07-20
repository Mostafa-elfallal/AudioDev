/*
 * player.c
 *
 *  Created on: May 20, 2025
 *      Author: mosta
 */

#include "player.h"
#include <stdio.h>
#include "stm32f4xx_hal.h"
#include "VS1053.h"
// #include "SRC4190.h"
#include "MUSES72320.h"
#include "math.h"
#include "fatfs.h"
#include "states.h"


#define BUFFER_SIZE 32

static uint8_t mp3Buffer[BUFFER_SIZE];
static uint32_t mp3FileSize;
static uint32_t readBytes;
static uint16_t cnt = 0;
static uint16_t volume = 100;

static osThreadId AudioTaskHandle;
static void AudioTaskFunc(void const *argument);
static osThreadDef(AudioTask, AudioTaskFunc, osPriorityNormal, 0, 1024);

FIL mp3File;

// static const uint8_t *file;

static bool _isFileOpen = false;
static bool _isPlaying = false;
static bool _isMuted = true;

static void Oscillator_ON(void)
{
	HAL_GPIO_WritePin(TXCO_ON_OFF_GPIO_Port, TXCO_ON_OFF_Pin, GPIO_PIN_SET);
}
__attribute__((unused))  static void Oscillator_OFF(void)
{
	HAL_GPIO_WritePin(TXCO_ON_OFF_GPIO_Port, TXCO_ON_OFF_Pin, GPIO_PIN_RESET);
}

static int16_t percent_to_dV(uint16_t percent)
{
	float gain = percent / 100.0f;
	float dB = 40.0f * log10f(gain); // Multiply by 2*20 = 40
	return (int16_t)roundf(dB);		 // Round to nearest int
}
static bool MP3_start_play()
{
	printf("[Player] Starting to play a file\r\n");
	if (!VS1053_SetMode(VS1053_MODE_SM_LINE1 | VS1053_MODE_SM_SDINEW)) /* SM LINE1 | SM SDINEW */
	{
		printf("[Player] Failed to Set Mode\r\n");
		return false;
	}
	if (!VS1053_AutoResync()) /* AutoResync */
	{
		printf("[Player] Failed to AutoResync\r\n");
		return false;
	}
	if (!VS1053_SetDecodeTime(0)) /* Set decode time */
	{
		printf("[Player] Failed to SetDecodeTime\r\n");
		return false;
	}
	if (!VS1053_SetVolume(0, 0)) /* Small number is louder */
	{
		printf("[Player] Failed to SetVolume\r\n");
		return false;
	}
	if (!VS1053_I2S_En())
	{
		printf("[Player] Failed to Enable I2S\r\n");
		return false;
	}
	return true;
}
bool MP3_isPlaying(void)
{
	return _isPlaying;
}
/* Initialize VS1053 & Open a file */
bool MP3_Init()
{
	//	printf("[Player] init SRC4190\r\n");
	//	src4190_init();
	printf("[Player] Oscillator On\r\n");
#ifndef USING_DEBUG_ONLY
	Oscillator_ON();
#endif
	printf("[Player] Wait 2 Seconds \r\n");
	HAL_Delay(2000);
	/* Initialize VS1053 */
	printf("[Player] init VS1053\r\n");
#ifndef USING_DEBUG_ONLY
	if (!VS1053_Init())
	{
		printf("[Player] Failed to init VS1053\r\n");
		return false;
	}
	muses_Init(MUSES_MODE_LR_LINKED_ATT |
			   MUSES_MODE_LR_LINKED_GAIN |
			   MUSES_MODE_ZC_DISABLED);
#endif
	//	src4190_Reset();
	return true;
}
bool MP3_Play(const char *filename)
{
#ifndef USING_DEBUG_ONLY
	if (!MP3_start_play())
	{
		printf("[Player] Failed to start playing\r\n");
		return false;
	}
	//	/* Open file to read */
	if (f_open(&mp3File, filename, FA_READ) != FR_OK)
		return false;
#endif
	//
	/* Get the file size */
	mp3FileSize = f_size(&mp3File);
	// file = filename;
	// mp3FileSize = size;

	readBytes = 0;
	/* Set flags */
	_isFileOpen = true;
	_isPlaying = true;
	if(!AudioTaskHandle)
	AudioTaskHandle = osThreadCreate(osThread(AudioTask), NULL);
	return true;
}
void MP3_Stop(void)
{
	/* Refer to page 49 of VS1053 datasheet */
	printf("[Player] Stop\r\n");
	uint16_t mode;
#ifndef USING_DEBUG_ONLY
	VS1053_SendEndFill(2052);															  /* send endfill bytes */
	VS1053_SetMode(VS1053_MODE_SM_LINE1 | VS1053_MODE_SM_SDINEW | VS1053_MODE_SM_CANCEL); /* SM LINE1 | SM SDINEW | SM CANCEL */
	VS1053_SendEndFill(32);																  /* send endfill bytes */
	HAL_Delay(100);
	VS1053_GetMode(&mode);					   /* get mode value */
	if ((mode & VS1053_MODE_SM_CANCEL) != 0x0) /* if SM CANCEL is not clear, soft reset */
	{
		VS1053_SoftReset();
	}
#endif
	f_close(&mp3File);
	_isPlaying = false;	 /* Stop flag */
	_isFileOpen = false; /* Close flag */
}

void MP3_Pause(void)
{
	printf("[Player] Pause\r\n");
	if (_isPlaying)
		_isPlaying = false;
}

void MP3_Resume(void)
{
	printf("[Player] Resume\r\n");
	if (!_isPlaying)
		_isPlaying = true;
}

/* Send mp3 buffer to VS1053 */
void MP3_Feeder(void)
{
	uint32_t remaining = mp3FileSize - readBytes;
	if (remaining > BUFFER_SIZE)
	{
		/* Fill the buffer */
		f_read(&mp3File, mp3Buffer, BUFFER_SIZE, (void *)&readBytes);

		// for (uint8_t i = 0; i < BUFFER_SIZE; i++)
		// {
		// 	mp3Buffer[i] = file[readBytes + i];
		// }
#ifdef USING_DEBUG_ONLY
		/* Tx buffer */
		VS1053_SdiWrite32(mp3Buffer);
#endif
		/* bytes to send */
		readBytes += BUFFER_SIZE;
	}
	else
	{
		/* Read left bytes */
		f_read(&mp3File, mp3Buffer, mp3FileSize, (void *)&readBytes);
		// for (uint8_t i = 0; i < remaining; i++)
		// {
		// 	mp3Buffer[i] = file[readBytes + i];
		// }
		/* Tx buffer */
#ifndef USING_DEBUG_ONLY
		for (cnt = 0; cnt < remaining; cnt++)
		{
			while (!VS1053_SdiWrite(*(mp3Buffer + cnt)))
				;
		}
#endif
		/* Stop when played the whole file */
		MP3_Stop();
		sm_post_event(&sm_ctx, EVENT_FINISHED_PLAYBACK, NULL); // Post event to indicate playback finished
		printf("[Player] Finished playing the file\r\n");
		if (AudioTaskHandle != NULL)
		{
			AudioTaskHandle = NULL;
			osThreadTerminate(NULL);

		}
	}
}
void MP3_Mute(void)
{
	printf("[Player] Mute\r\n");
#ifndef USING_DEBUG_ONLY
	muses_Mute();
#endif
	_isMuted = true;
}
void MP3_unMute(void)
{
	printf("[Player] unMute\r\n");
#ifndef USING_DEBUG_ONLY
	muses_UnMute();
#endif
	_isMuted = false;
}
bool MP3_isMuted(void)
{
	return _isMuted;
}
void MP3_setVolume(uint16_t vPercentage)
{
	if (vPercentage > 200)
	{
		printf("[Player] Out Of range Percentage : %d\r\n", vPercentage);
		return;
	}
	volume = vPercentage;
	if (volume == 0)
	{
		MP3_Mute();
	}
	else
	{
		int16_t vol_dV = percent_to_dV(volume);
		printf("[Player] Setting Volume = %d dB\r\n", vol_dV);
#ifndef USING_DEBUG_ONLY
		muses_setVolume(vol_dV);
#endif
		if (MP3_isMuted())
		{
			MP3_unMute();
		}
	}

	printf("[Player] New Volume = %d \r\n", volume);
}
uint16_t MP3_getVolume()
{
	return volume;
}
void MP3_addVolume(int16_t delta)
{
	int16_t newVolume = (int16_t)volume + delta;
	if (newVolume <= 0)
		MP3_setVolume(0);
	else
		MP3_setVolume((uint16_t)newVolume);
}
static void AudioTaskFunc(void const *argument)
{
    for (;;)
    {

    	bool busy = false ;
#ifndef USING_DEBUG_ONLY
    	busy = VS1053_IsBusy();
#endif
	   if (busy || !_isPlaying || !_isFileOpen)
				osDelay(10);
       else
	   {
#ifdef USING_DEBUG_ONLY
    	   osDelay(5000);
#endif
			MP3_Feeder();
	   }

    }
}
