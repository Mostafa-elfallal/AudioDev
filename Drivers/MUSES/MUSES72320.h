
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
// FILE:	MUSES72320.h
//
// TITLE:	MUSES72320 +/- 18V Operation 2-Channel Electronic Volume
//
//#########################################################################*/
//(1): Header files

#ifndef MUSES72320_H_
#define MUSES72320_H_

#include "stm32f4xx_hal.h"
#include <stdbool.h>

//MUSES72320 Register Map
/*MUSES72320 control data is constructed with 16bits [DATA(8), Select Address(4), Chip Address(4)]*/
#define MUSES_C_ADD    		0x00 			// MUSES Chip Address
#define MUSES_LVC_At   		0x00 			// Lch Volume Control (Att.)
#define MUSES_LVC_G    		(1<<4) 			// Lch Volume Control (Gain)
#define MUSES_RVC_At   		(1<<5)			// Rch Volume Control (Att.)
#define MUSES_RVC_G    		((1<<4)|(1<<5))	// Rch Volume Control (Gain)
#define MUSES_CONFIG		(1<<6)			// BIT 7 toconfigure MUSES parameters (ZC, L/R gain & att link)

//(1): MUSES Mode
#define MUSES_MODE_LR_INDEPENDENT 	(0)
#define MUSES_MODE_LR_LINKED_ATT	(1<<15)
#define MUSES_MODE_LR_LINKED_GAIN	(1<<14)
#define MUSES_MODE_ZC_DISABLED		(1<<13)


void muses_Init( uint16_t modes);
void muses_Mute(void);
void muses_UnMute(void);
bool muses_isMute(void);
/**
 * volume starts with -6 dB (~50%)
 * can be -111 to +31
 * I created a new unit which is dV
 * 2 dV = 1 dB to include 0.5 dB without using floats
 * */
void muses_addVolume(int16_t delta); // delta 2dV = 1dB
int16_t muses_getVolume(); // in dV
void muses_setVolume(int16_t dV);
#endif /* MUSES72320_H_ */


