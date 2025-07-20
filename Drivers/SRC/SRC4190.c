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
// FILE:	SRC4190.c
//
// TITLE:	Stereo Asynchronous Sample-Rate Converters SRC4190 configuration
//
//#########################################################################*/

#include "SRC4190.h"

#include "stm32f4xx_hal.h"
#include "main.h"
#include <stdio.h>
void src4190_setAudPrtMode(SRC4190_MODE audioMode)
{
	switch (audioMode)
	{
		case MODE_SRC_IOP_Slv:
			printf("[SRC74190] AudPrtMode IOP_Slv\r\n");
			HAL_GPIO_WritePin(SRC_MODE0_GPIO_Port,SRC_MODE0_Pin,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(SRC_MODE1_GPIO_Port,SRC_MODE1_Pin,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(SRC_MODE2_GPIO_Port,SRC_MODE2_Pin,GPIO_PIN_RESET);
			break;
		case MODE_SRC_OP_Ms_128fs:
			printf("[SRC74190] AudPrtMode OP_Ms_128fs\r\n");
			HAL_GPIO_WritePin(SRC_MODE0_GPIO_Port,SRC_MODE0_Pin,GPIO_PIN_SET);
			HAL_GPIO_WritePin(SRC_MODE1_GPIO_Port,SRC_MODE1_Pin,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(SRC_MODE2_GPIO_Port,SRC_MODE2_Pin,GPIO_PIN_RESET);
			break;
		case MODE_SRC_OP_Ms_512fs:
			printf("[SRC74190] AudPrtMode OP_Ms_512fs\r\n");
			HAL_GPIO_WritePin(SRC_MODE0_GPIO_Port,SRC_MODE0_Pin,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(SRC_MODE1_GPIO_Port,SRC_MODE1_Pin,GPIO_PIN_SET);
			HAL_GPIO_WritePin(SRC_MODE2_GPIO_Port,SRC_MODE2_Pin,GPIO_PIN_RESET);
			break;
		case MODE_SRC_OP_Ms_256fs:
			printf("[SRC74190] AudPrtMode OP_Ms_256fs\r\n");
			HAL_GPIO_WritePin(SRC_MODE0_GPIO_Port,SRC_MODE0_Pin,GPIO_PIN_SET);
			HAL_GPIO_WritePin(SRC_MODE1_GPIO_Port,SRC_MODE1_Pin,GPIO_PIN_SET);
			HAL_GPIO_WritePin(SRC_MODE2_GPIO_Port,SRC_MODE2_Pin,GPIO_PIN_RESET);
			break;
		case MODE_SRC_IP_Ms_128fs:
			printf("[SRC74190] AudPrtMode IP_Ms_128fs\r\n");
			HAL_GPIO_WritePin(SRC_MODE0_GPIO_Port,SRC_MODE0_Pin,GPIO_PIN_SET);
			HAL_GPIO_WritePin(SRC_MODE1_GPIO_Port,SRC_MODE1_Pin,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(SRC_MODE2_GPIO_Port,SRC_MODE2_Pin,GPIO_PIN_SET);
			break;
		case MODE_SRC_IP_Ms_512fs:
			printf("[SRC74190] AudPrtMode IP_Ms_512fs\r\n");
			HAL_GPIO_WritePin(SRC_MODE0_GPIO_Port,SRC_MODE0_Pin,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(SRC_MODE1_GPIO_Port,SRC_MODE1_Pin,GPIO_PIN_SET);
			HAL_GPIO_WritePin(SRC_MODE2_GPIO_Port,SRC_MODE2_Pin,GPIO_PIN_SET);
			break;
		case MODE_SRC_IP_Ms_256fs:
			printf("[SRC74190] AudPrtMode IP_Ms_256fs\r\n");
			HAL_GPIO_WritePin(SRC_MODE0_GPIO_Port,SRC_MODE0_Pin,GPIO_PIN_SET);
			HAL_GPIO_WritePin(SRC_MODE1_GPIO_Port,SRC_MODE1_Pin,GPIO_PIN_SET);
			HAL_GPIO_WritePin(SRC_MODE2_GPIO_Port,SRC_MODE2_Pin,GPIO_PIN_SET);
			break;	
	}
}

bool src4190_IsReady(void)
{
	if (HAL_GPIO_ReadPin(SRC_RDY_GPIO_Port,SRC_RDY_Pin)) return false;
	else return true;
}

void src4190_setInPrtDataFrmt(SRC4190_InPrtDataFrmt inDataFormat)
{
	switch (inDataFormat)
	{
		case DATA_FRMT_LJ_24b:
			printf("[SRC74190] InPrtDataFrmt LJ_24b\r\n");
			HAL_GPIO_WritePin(SRC_IFM0_GPIO_Port,SRC_IFM0_Pin,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(SRC_IFM1_GPIO_Port,SRC_IFM1_Pin,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(SRC_IFM2_GPIO_Port,SRC_IFM2_Pin,GPIO_PIN_RESET);
			break;
		case DATA_FRMT_I2S_24b:
			printf("[SRC74190] InPrtDataFrmt I2S_24b\r\n");
			HAL_GPIO_WritePin(SRC_IFM0_GPIO_Port,SRC_IFM0_Pin,GPIO_PIN_SET);
			HAL_GPIO_WritePin(SRC_IFM1_GPIO_Port,SRC_IFM1_Pin,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(SRC_IFM2_GPIO_Port,SRC_IFM2_Pin,GPIO_PIN_RESET);
			break;
		case DATA_FRMT_RJ_16b:
			printf("[SRC74190] InPrtDataFrmt RJ_16b\r\n");
			HAL_GPIO_WritePin(SRC_IFM0_GPIO_Port,SRC_IFM0_Pin,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(SRC_IFM1_GPIO_Port,SRC_IFM1_Pin,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(SRC_IFM2_GPIO_Port,SRC_IFM2_Pin,GPIO_PIN_SET);
			break;
		case DATA_FRMT_RJ_18b:
			printf("[SRC74190] InPrtDataFrmt RJ_18b\r\n");
			HAL_GPIO_WritePin(SRC_IFM0_GPIO_Port,SRC_IFM0_Pin,GPIO_PIN_SET);
			HAL_GPIO_WritePin(SRC_IFM1_GPIO_Port,SRC_IFM1_Pin,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(SRC_IFM2_GPIO_Port,SRC_IFM2_Pin,GPIO_PIN_SET);
			break;
		case DATA_FRMT_RJ_20b:
			printf("[SRC74190] InPrtDataFrmt RJ_20b\r\n");
			HAL_GPIO_WritePin(SRC_IFM0_GPIO_Port,SRC_IFM0_Pin,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(SRC_IFM1_GPIO_Port,SRC_IFM1_Pin,GPIO_PIN_SET);
			HAL_GPIO_WritePin(SRC_IFM2_GPIO_Port,SRC_IFM2_Pin,GPIO_PIN_SET);
			break;
		case DATA_FRMT_RJ_24b:
			printf("[SRC74190] InPrtDataFrmt RJ_24b\r\n");
			HAL_GPIO_WritePin(SRC_IFM0_GPIO_Port,SRC_IFM0_Pin,GPIO_PIN_SET);
			HAL_GPIO_WritePin(SRC_IFM1_GPIO_Port,SRC_IFM1_Pin,GPIO_PIN_SET);
			HAL_GPIO_WritePin(SRC_IFM2_GPIO_Port,SRC_IFM2_Pin,GPIO_PIN_SET);
			break;
		case DATA_FRMT_UNUSED:
			printf("[SRC74190] InPrtDataFrmt UNUSED\r\n");
			HAL_GPIO_WritePin(SRC_IFM0_GPIO_Port,SRC_IFM0_Pin,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(SRC_IFM1_GPIO_Port,SRC_IFM1_Pin,GPIO_PIN_SET);
			HAL_GPIO_WritePin(SRC_IFM2_GPIO_Port,SRC_IFM2_Pin,GPIO_PIN_RESET);
			break;
	}
}

void src4190_setOutPrtDataFrmt(SRC4190_OutPrtDataFrmt outDataFormat, SRC4190_OutDataWrdLength wordLength)
{
	switch (outDataFormat)
	{
		case DATA_FRMT_LJ:
			printf("[SRC74190] OutPrtDataFrmt LJ\r\n");
			HAL_GPIO_WritePin(SRC_OFM0_GPIO_Port,SRC_OFM0_Pin,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(SRC_OFM1_GPIO_Port,SRC_OFM1_Pin,GPIO_PIN_RESET);
			break;
		case DATA_FRMT_I2S:
			printf("[SRC74190] OutPrtDataFrmt I2S\r\n");
			HAL_GPIO_WritePin(SRC_OFM0_GPIO_Port,SRC_OFM0_Pin,GPIO_PIN_SET);
			HAL_GPIO_WritePin(SRC_OFM1_GPIO_Port,SRC_OFM1_Pin,GPIO_PIN_RESET);
			break;
		case DATA_FRMT_RJ:
			printf("[SRC74190] OutPrtDataFrmt RJ\r\n");
			HAL_GPIO_WritePin(SRC_OFM0_GPIO_Port,SRC_OFM0_Pin,GPIO_PIN_SET);
			HAL_GPIO_WritePin(SRC_OFM1_GPIO_Port,SRC_OFM1_Pin,GPIO_PIN_SET);
			break;
		case DATA_FRMT_TDM:
			printf("[SRC74190] OutPrtDataFrmt TDM\r\n");
			HAL_GPIO_WritePin(SRC_OFM0_GPIO_Port,SRC_OFM0_Pin,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(SRC_OFM1_GPIO_Port,SRC_OFM1_Pin,GPIO_PIN_SET);
			break;
	}
	src4190_setOutDataWrdLength(wordLength);
}

void src4190_setOutDataWrdLength(SRC4190_OutDataWrdLength wordLength)
{
	switch (wordLength)
	{
		case DATA_24b:
			printf("[SRC74190] OutDataWrdLength 24b\r\n");
			HAL_GPIO_WritePin(SRC_OWL0_GPIO_Port,SRC_OWL0_Pin,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(SRC_OWL1_GPIO_Port,SRC_OWL1_Pin,GPIO_PIN_RESET);
			break;
		case DATA_20b:
			printf("[SRC74190] OutDataWrdLength 20b\r\n");
			HAL_GPIO_WritePin(SRC_OWL0_GPIO_Port,SRC_OWL0_Pin,GPIO_PIN_SET);
			HAL_GPIO_WritePin(SRC_OWL1_GPIO_Port,SRC_OWL1_Pin,GPIO_PIN_RESET);
			break;
		case DATA_18b:
			printf("[SRC74190] OutDataWrdLength 18b\r\n");
			HAL_GPIO_WritePin(SRC_OWL0_GPIO_Port,SRC_OWL0_Pin,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(SRC_OWL1_GPIO_Port,SRC_OWL1_Pin,GPIO_PIN_SET);
			break;
		case DATA_16b:
			printf("[SRC74190] OutDataWrdLength 16b\r\n");
			HAL_GPIO_WritePin(SRC_OWL0_GPIO_Port,SRC_OWL0_Pin,GPIO_PIN_SET);
			HAL_GPIO_WritePin(SRC_OWL1_GPIO_Port,SRC_OWL1_Pin,GPIO_PIN_SET);
			break;
	}
}

void src4190_ByPass(void){
	printf("[SRC74190] ByPass\r\n");
	HAL_GPIO_WritePin(SRC_BYPASS_GPIO_Port,SRC_BYPASS_Pin,GPIO_PIN_SET);
}


void src4190_MUTE(void){
	HAL_GPIO_WritePin(SRC_MUTE_GPIO_Port,SRC_MUTE_Pin,GPIO_PIN_SET);
	printf("[SRC74190] Mute\r\n");
}

void src4190_UnMUTE(void){
	HAL_GPIO_WritePin(SRC_MUTE_GPIO_Port,SRC_MUTE_Pin,GPIO_PIN_RESET);
	printf("[SRC74190] unMute\r\n");
}
void src4190_Reset(void)
{
	printf("[SRC74190] reset\r\n");
	HAL_GPIO_WritePin(SRC_RST_GPIO_Port,SRC_RST_Pin,GPIO_PIN_RESET);
	HAL_Delay(100);
	HAL_GPIO_WritePin(SRC_RST_GPIO_Port,SRC_RST_Pin,GPIO_PIN_SET);
	HAL_Delay(100);
	HAL_GPIO_WritePin(SRC_RST_GPIO_Port,SRC_RST_Pin,GPIO_PIN_RESET);
	HAL_Delay(100);
	HAL_GPIO_WritePin(SRC_RST_GPIO_Port,SRC_RST_Pin,GPIO_PIN_SET);
	HAL_Delay(100);
}
void src4190_LowDelayGrp(bool en)
{
	printf("[SRC74190] LGRP : %s\r\n",en?"Enable" : "Disable");
	HAL_GPIO_WritePin(SRC_LGRP_GPIO_Port,SRC_LGRP_Pin,en);

}
void src4190_init(void){
	printf("[SRC74190] Init\r\n");
	src4190_setAudPrtMode(MODE_SRC_OP_Ms_256fs);
	src4190_setInPrtDataFrmt(DATA_FRMT_RJ_16b);
	src4190_setOutPrtDataFrmt(DATA_FRMT_I2S, DATA_24b);
	src4190_LowDelayGrp(false);
}
