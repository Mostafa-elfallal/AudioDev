/*
Library:					STM32h7 Stereo Asynchronous Sample-Rate Converters SRC4190
Written by:				Mostafa M. Samy
Date Written:			4/11/2024
Last modified:		4/11/2024
Description:			This is an STM32 device driver library for the SRC4190 Sample-Rate Converter, using STM HAL libraries

References:
			1) SRC4190 192-kHz Stereo Asynchronous Sample-Rate Converters datasheet
				 https://www.ti.com/lit/ds/symlink/tlv320aic23b.pdf
						
* Copyright (C) 2024 - M. Samy
   This is a free software under the GNU license, you can redistribute it and/or modify it under the terms
   of the GNU General Public Licenseversion 3 as published by the Free Software Foundation.
	
   This software library is shared with puplic for educational purposes, without WARRANTY and Author is not liable for any damages caused directly
   or indirectly by this software, read more about this on the GNU General Public License.
*/
/*###########################################################################
//
// FILE:	SRC4190.h
//
// TITLE:	Stereo Asynchronous Sample-Rate Converters SRC4190 configuration
//
//#########################################################################*/
//(1): Header files

#ifndef SRC4190_H_
#define SRC4190_H_

#include "stm32f4xx_hal.h"
#include <stdbool.h>


//(1): Mode Select Enum
typedef enum {
	MODE_SRC_IOP_Slv = 0,
	MODE_SRC_OP_Ms_128fs,
	MODE_SRC_OP_Ms_512fs,
	MODE_SRC_OP_Ms_256fs,
	MODE_SRC_IP_Ms_128fs,
	MODE_SRC_IP_Ms_512fs,
	MODE_SRC_IP_Ms_256fs,
}	SRC4190_MODE;

//(2): Input Port Data Format Select Enum
typedef enum {
	DATA_FRMT_LJ_24b = 0,
	DATA_FRMT_I2S_24b,
	DATA_FRMT_RJ_16b,
	DATA_FRMT_RJ_18b,
	DATA_FRMT_RJ_20b,
	DATA_FRMT_RJ_24b,
	DATA_FRMT_UNUSED,
}	SRC4190_InPrtDataFrmt;

//(3): Output Port Data Format Select Enum
typedef enum {
	DATA_FRMT_LJ = 0,
	DATA_FRMT_I2S,
	DATA_FRMT_RJ,
	DATA_FRMT_TDM,
}	SRC4190_OutPrtDataFrmt;

//(4): Output Data Word Length Enum
typedef enum {
	DATA_24b = 0,
	DATA_20b,
	DATA_18b,
	DATA_16b,
}	SRC4190_OutDataWrdLength;

/**
 * @brief Set SRC4190 Audio Port Mode
 * @param audioMode - Audio port mode type definition
 */
void src4190_setAudPrtMode(SRC4190_MODE audioMode);

/**
 * @brief Get SRC4190 ready state
					The SRC4190 includes an active low ready output (RDY). This is an output from the rate estimator block, which
					indicates that the input-to-output sampling frequency ratio has been determined. The ready signal can be used
					as a flag or indicator output. The ready signal can also be connected to the active high MUTE pin to provide an
					auto-mute function, so that the output port is muted when the rate estimator is in transition.
 * @param True if the input-to-output sampling frequency ratio has been determined.
 */
bool src4190_IsReady(void);

/**
 * @brief Set SRC4190 Input Port Data Format
 * @param dataFormat - Input Port Data Format type definition
 */
void src4190_setInPrtDataFrmt(SRC4190_InPrtDataFrmt inDataFormat);
/**/

/**
 * @brief Set SRC4190 Output Port Data Format
 * @param dataFormat - Output Port Data Format type definition
 */
void src4190_setOutPrtDataFrmt(SRC4190_OutPrtDataFrmt outDataFormat, SRC4190_OutDataWrdLength wordLength);
/**/

/**
 * @brief Set SRC4190 Output Data Word Length
 * @param wordLength - Output Port Data Word Length type definition
 */
void src4190_setOutDataWrdLength(SRC4190_OutDataWrdLength wordLength);
/**/

/**
 * @brief By-Pass SRC4190 asynchronous sample rate converter (ASRC)
					//A bypass mode  allows audio data to be passed directly from the input port to the output port,
					//bypassing the ASRC function. The bypass option is useful for passing through encoded or 
					//compressed audio data, or nonaudio control or status data.
 * @param NULL.
 */
void src4190_ByPass(void);

/**
 * @brief Mute SRC4190 Output
					//A soft mute function is available providing artifact-free operation while muting the audio output signal.
					//The mute attenuation is typically �128 dB.
 * @param NULL.
 */
void src4190_MUTE(void);

/**
 * @brief UnMute SRC4190 Output
 * @param NULL.
 */
void src4190_UnMUTE(void);
void src4190_Reset(void);
void src4190_LowDelayGrp(bool en);
void src4190_init(void);
#endif /* SRC4190_H_ */
