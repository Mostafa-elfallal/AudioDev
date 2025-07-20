/*
Library:					MUSES72320 configurationibrary
Written by:				Mostafa M. Samy
Date Written:			17/11/2024
Last modified:		17/11/2024
Description:			This is an STM32 device driver library for the MUSES72320 +/- 18V Operation 2-Channel Electronic Volume

References:
			1) MUSES72320 +/- 18V Operation 2-Channel Electronic Volume
				 https://www.
						
* Copyright (C) 2024 - M. Samy
   This is a free software under the GNU license, you can redistribute it and/or modify it under the terms
   of the GNU General Public Licenseversion 3 as published by the Free Software Foundation.
	
   This software library is shared with puplic for educational purposes, without WARRANTY and Author is not liable for any damages caused directly
   or indirectly by this software, read more about this on the GNU General Public License.
*/
/*###########################################################################
//
// FILE:	MUSES72320.c
//
// TITLE:	MUSES72320 +/- 18V Operation 2-Channel Electronic Volume
//
//#########################################################################*/
#include "MUSES72320.h"
#include "stm32f4xx_hal.h"
#include "main.h"
#include <stdio.h>
extern SPI_HandleTypeDef hspi2;
#define SPI hspi2 // 16-bit mode

static int16_t g_volume = -6; // 2x the required dB to include 0.5
static uint16_t g_config = 0;
static bool g_isMute = true;

//(1): Functions definitions
//-------------- Static Functions ---------------//
// Function(1): Write to register
static void write_register(uint16_t val)
{

	HAL_GPIO_WritePin(MUSES_LATCH_GPIO_Port, MUSES_LATCH_Pin, GPIO_PIN_RESET);

	//HAL_SPI_Transmit(spix, volData, 2, HAL_MAX_DELAY);
	HAL_StatusTypeDef MUSES_status = HAL_SPI_Transmit(&SPI, (uint8_t*)&val, 1, HAL_MAX_DELAY);
	(void)MUSES_status; // supress warning
	for(volatile int i = 0;i<100;i++) ; // some delay for latching
	HAL_GPIO_WritePin(MUSES_LATCH_GPIO_Port, MUSES_LATCH_Pin, GPIO_PIN_SET);;
}
static void mute_to_regVals(uint16_t *atVal,uint16_t *gVal)
{
	*atVal = 0;
	*gVal = 0;
}
static void db_to_regVals(int16_t db,uint16_t *atVal,uint16_t *gVal)
{
	if(db > 0) // gain
	{
		*atVal = 0;
		*gVal = ((uint16_t)db) << 8;
	}
	else{		// atten or 0
		*gVal = 0;
		*atVal = (((uint16_t)(-db)) + 0x10)  << 8;
	}
}
static void writeVolume(uint16_t att,uint16_t gain)
{
	uint16_t leftAtt 	= att 	| MUSES_LVC_At 	| MUSES_C_ADD;
	uint16_t rightAtt 	= att 	| MUSES_RVC_At 	| MUSES_C_ADD;
	uint16_t leftGain 	= gain 	| MUSES_LVC_G 	| MUSES_C_ADD;
	uint16_t rightGain 	= gain 	| MUSES_RVC_G 	| MUSES_C_ADD;

	write_register(leftAtt);
	write_register(leftGain);
	if(! (g_config & MUSES_MODE_LR_LINKED_ATT))
	{
		write_register(rightAtt);
	}
	if(! (g_config & MUSES_MODE_LR_LINKED_GAIN))
	{
		write_register(rightGain);
	}
}
//-------------- Public Functions ----------------//
// Function(1): Initialisation
void muses_Init(uint16_t modes)
{

	printf("[Muses] Init\r\n");
	uint16_t confVal = modes | MUSES_CONFIG | MUSES_C_ADD;
	write_register(confVal);
	g_config = modes;
}

// Function(2): MUTE
void muses_Mute(void)
{
	printf("[Muses] Mute\r\n");
	uint16_t att,gain;
	mute_to_regVals(&att,&gain);
	writeVolume(att,gain);
	g_isMute = true;
}

// Function(2): UNMUTE
void muses_UnMute(void)
{
	printf("[Muses] unMute\r\n");
	uint16_t att,gain;
	db_to_regVals(g_volume,&att,&gain);
	writeVolume(att,gain);
	g_isMute = false;
}
bool muses_isMute(void)
{
	return g_isMute;
}
void muses_addVolume(int16_t dV)
{
	int16_t newVol = g_volume + dV;
	printf("[Muses] New Volume = %d\r\n",newVol);
	muses_setVolume(newVol);
}
int16_t muses_getVolume()
{
	return g_volume;
}
void muses_setVolume(int16_t V)
{
	if(V >= -223 && V <= 63){
		g_volume = V;
	}

}



