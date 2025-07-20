/*
 * VS1053.h
 *
 *  Created on: May 20, 2025
 *      Author: mosta
 */

#ifndef VS1053_VS1053_H_
#define VS1053_VS1053_H_


#include "stm32f4xx_hal.h"
#include <stdint.h>
#include <stdbool.h>
#include "stm32f4xx_hal.h"
#include "main.h"
/* Pin configuration */
extern SPI_HandleTypeDef 			hspi1;
#define HSPI_VS1053					&hspi1

/* Registers */
#define VS1053_REG_BASE	 		0x00
#define VS1053_REG_MODE   	 	0x00
#define VS1053_REG_STATUS 	 	0x01
#define VS1053_REG_BASS 		0x02
#define VS1053_REG_CLOCKF 		0x03
#define VS1053_REG_DECODE_TIME 	0x04
#define VS1053_REG_AUDATA 	 	0x05
#define VS1053_REG_WRAM 		0x06
#define VS1053_REG_WRAMADDR 	0x07
#define VS1053_REG_HDAT0 		0x08
#define VS1053_REG_HDAT1 		0x09
#define VS1053_REG_AIADDR 	 	0x0A
#define VS1053_REG_VOL 		 	0x0B
#define VS1053_REG_AICTRL0 	 	0x0C
#define VS1053_REG_AICTRL1 	 	0x0D
#define VS1053_REG_AICTRL2 	 	0x0E
#define VS1053_REG_AICTRL3 	 	0x0F
#define VS1053_MODE_SM_DIFF     0x0001 //!< Differential, 0: normal in-phase audio, 1: left channel inverted
#define VS1053_MODE_SM_LAYER12 	0x0002  //!< Allow MPEG layers I & II
#define VS1053_MODE_SM_RESET 	0x0004    //!< Soft reset
#define VS1053_MODE_SM_CANCEL 	0x0008   //!< Cancel decoding current file
#define VS1053_MODE_SM_EARSPKLO 0x0010 //!< EarSpeaker low setting
#define VS1053_MODE_SM_TESTS 	0x0020    //!< Allow SDI tests
#define VS1053_MODE_SM_STREAM 	0x0040   //!< Stream mode
#define VS1053_MODE_SM_SDINEW 	0x0800   //!< VS1002 native SPI modes
#define VS1053_MODE_SM_ADPCM 	0x1000    //!< PCM/ADPCM recording active
#define VS1053_MODE_SM_LINE1 	0x4000 //!< MIC/LINE1 selector, 0: MICP, 1: LINE1

#define VS1053_DREQ_PORT			MP3_DREQ_GPIO_Port
#define VS1053_DREQ_PIN				MP3_DREQ_Pin
#define	VS1053_XRST_PORT			MP3_RST_GPIO_Port
#define	VS1053_XRST_PIN				MP3_RST_Pin
#define VS1053_XCS_PORT				MP3_CS_GPIO_Port
#define VS1053_XCS_PIN				MP3_CS_Pin
#define VS1053_XDCS_PORT			MP3_XDSC_GPIO_Port
#define VS1053_XDCS_PIN				MP3_XDSC_Pin


/* Functions */
bool VS1053_Init();
void VS1053_Reset();
bool VS1053_SoftReset();
bool VS1053_SetVolume(uint8_t volumeLeft, uint8_t volumeRight);
bool VS1053_SetMode(uint16_t mode);
bool VS1053_GetMode(uint16_t *mode);
bool VS1053_AutoResync();
bool VS1053_SetDecodeTime(uint16_t time);
bool VS1053_SendEndFill(uint16_t num);
bool VS1053_IsBusy();
bool VS1053_SciWrite(uint8_t address, uint16_t input);
bool VS1053_SciRead(uint8_t address, uint16_t *res);
bool VS1053_SdiWrite(uint8_t input);
bool VS1053_SdiWrite8(uint8_t *input8);
bool VS1053_SdiWrite32(uint8_t *input32);
bool VS1053_I2S_En(void);
void VS1053_SineTestStart(void);
void VS1053_SineTestStop(void);
extern uint8_t endFillByte;

#endif /* VS1053_VS1053_H_ */
