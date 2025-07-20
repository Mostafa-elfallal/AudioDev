/*
 * VS1053.c
 *
 *  Created on: May 20, 2025
 *      Author: mosta
 */
#include "vs1053.h"
#include <stdio.h>
/* Commands */
#define VS1053_WRITE_CMD	0x02;
#define VS1053_READ_CMD		0x03;



/* Pin control */
#define XCS_HIGH	HAL_GPIO_WritePin(VS1053_XCS_PORT, VS1053_XCS_PIN, GPIO_PIN_SET)
#define XCS_LOW		HAL_GPIO_WritePin(VS1053_XCS_PORT, VS1053_XCS_PIN, GPIO_PIN_RESET)
#define XDCS_HIGH	HAL_GPIO_WritePin(VS1053_XDCS_PORT, VS1053_XDCS_PIN, GPIO_PIN_SET)
#define XDCS_LOW	HAL_GPIO_WritePin(VS1053_XDCS_PORT, VS1053_XDCS_PIN, GPIO_PIN_RESET)
#define XRST_HIGH	HAL_GPIO_WritePin(VS1053_XRST_PORT, VS1053_XRST_PIN, GPIO_PIN_SET)
#define XRST_LOW	HAL_GPIO_WritePin(VS1053_XRST_PORT, VS1053_XRST_PIN, GPIO_PIN_RESET)

/* endFill byte is required to stop playing */
uint8_t endFillByte;

/* Initialize VS1053 */
bool VS1053_Init()
{
	uint16_t status = 0;

	XCS_HIGH;		    /* XCS High */
	XDCS_HIGH;		    /* XDCS High */
	/* x 1.0 Clock, 12MHz / 7, SPI Baudrate should be less than 1.75MHz */
	(HSPI_VS1053)->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;  /* 42MHz / 32 = 1.31MHz */
	if(HAL_SPI_Init(HSPI_VS1053) != HAL_OK)
	{
		printf("[VS1053] Failed to Init SPI\r\n");
		return false;
	}
	VS1053_Reset();     /* Hard Reset */



	/* Read Status to check SPI */
	if(!VS1053_SciRead(VS1053_REG_STATUS, &status))
	{
		printf("[VS1053] Failed to Read Status Reg\r\n");
		return false;
	}
	if(((status >> 4) & 0x0F) != 0x04)
	{
		printf("[VS1053] status value isn't correct\r\n");
		return false;
	}

	/* MP3 Mode GPIO configuration */
	if(!VS1053_SciWrite(VS1053_REG_WRAMADDR, 0xC017)) /* GPIO direction */
	{
		printf("[VS1053] Failed to Write 0xC017 to WRAMADDR\r\n");
		return false;
	}
	if(!VS1053_SciWrite(VS1053_REG_WRAM, 3))
	{
		printf("[VS1053] Failed to Write to WRAM\r\n");
		return false;
	}
	if(!VS1053_SciWrite(VS1053_REG_WRAMADDR, 0xC019)) /* GPIO output */
	{
		printf("[VS1053] Failed to Write 0xC019 to WRAMADDR\r\n");
		return false;
	}
	if(!VS1053_SciWrite(VS1053_REG_WRAM, 0))
	{
		printf("[VS1053] Failed to Write to WRAM\r\n");
		return false;
	}

	/* Soft reset */
	if(!VS1053_SoftReset())
	{
		printf("[VS1053] Failed to soft reset\r\n");
		return false;
	}

	/* x3.0 Clock, 36MHz / 7, SPI Baudrate should be less than 5.14MHz */
	if(!VS1053_SciWrite(VS1053_REG_CLOCKF, 0x6000))
	{
		printf("[VS1053] Failed to Write 0x6000 to CLOCKF\r\n");
		return false;
	}

	(HSPI_VS1053)->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;  /* 42MHz / 16 = 2.625MHz */
	if(HAL_SPI_Init(HSPI_VS1053) != HAL_OK)
	{
		printf("[VS1053] Failed to Init SPI\r\n");
		return false;
	}

	/* Read Status to check SPI */
	if(!VS1053_SciRead(VS1053_REG_STATUS, &status))
	{
		printf("[VS1053] Failed to Read Status Reg\r\n");
		return false;
	}
	if(((status >> 4) & 0x0F) != 0x04)
	{
		printf("[VS1053] status value isn't correct\r\n");
		return false;
	}
	/* Read endFill Byte */
	uint16_t regVal;
	if(!VS1053_SciWrite(VS1053_REG_WRAMADDR, 0x1E06))/* endFill */
	{
		printf("[VS1053] Failed to Write 0x1E06 to WRAMADDR\r\n");
		return false;
	}
	if(!VS1053_SciRead(VS1053_REG_WRAM, &regVal))
	{
		printf("[VS1053] Failed to Read from WRAM\r\n");
		return false;
	}
	endFillByte = regVal & 0xFF;

	return true;
}

/* Hard reset */
void VS1053_Reset()
{
	uint8_t dummy = 0xFF;
	XRST_LOW;		                                    /* XRST Low */
	HAL_SPI_Transmit(HSPI_VS1053, &dummy, 1, 10);       /* Tx Dummy */
	HAL_Delay(10);										/* 10ms Delay */
	XRST_HIGH;			                                /* XRST High */
	HAL_Delay(10);
}

/* Soft reset */
bool VS1053_SoftReset()
{
	if(!VS1053_SciWrite(VS1053_REG_MODE, 0x4804))	/* SM LINE1 | SM SDINEW | SM RESET */
	{
		printf("[VS1053] Failed to Write 0x4804 to MODE\r\n");
		return false;
	}
	HAL_Delay(100);
	return true;
}

/* Volume control */
bool VS1053_SetVolume(uint8_t volumeLeft, uint8_t volumeRight)
{
    uint16_t volume;
    volume = ( volumeLeft << 8 ) + volumeRight;

    if(!VS1053_SciWrite(VS1053_REG_VOL, volume))
	{
		printf("[VS1053] Failed to Write 0x%X to Vol\r\n",volume);
		return false;
	}
    return true;
}

/* Mode control */
bool VS1053_SetMode(uint16_t mode)
{
	if(!VS1053_SciWrite(VS1053_REG_MODE, mode))
	{
		printf("[VS1053] Failed to Write 0x%X to Mode\r\n",mode);
		return false;
	}
	return true;
}

bool VS1053_GetMode(uint16_t *mode)
{
	if(!VS1053_SciRead(VS1053_REG_MODE, mode))
	{
		printf("[VS1053] Failed to Read Mode\r\n");
		return false;
	}
	return true;
}

/* Resync control */
bool VS1053_AutoResync()
{
	if(!VS1053_SciWrite(VS1053_REG_WRAMADDR, 0x1E29)) /* Auto Resync */
	{
		printf("[VS1053] Failed to Write 0x1E29 to WRAMADDR\r\n");
		return false;
	}
	if(!VS1053_SciWrite(VS1053_REG_WRAM, 0))
	{
		printf("[VS1053] Failed to Write 0 to WRAM\r\n");
		return false;
	}
	return true;
}

/* Set decode time */
bool VS1053_SetDecodeTime(uint16_t time)
{
	if(!VS1053_SciWrite(VS1053_REG_DECODE_TIME, time))
	{
		printf("[VS1053] Failed to Write 0x%X to DECODE_TIME\r\n",time);
		return false;
	}
	if(!VS1053_SciWrite(VS1053_REG_DECODE_TIME, time))
	{
		printf("[VS1053] Failed to Write 0x%X to DECODE_TIME\r\n",time);
		return false;
	}
	return true;
}

/* Send endfill bytes */
bool VS1053_SendEndFill(uint16_t num)
{
	uint16_t regVal;
	if(!VS1053_SciWrite(VS1053_REG_WRAMADDR, 0x1E06))	/* endFill */
	{
		printf("[VS1053] Failed to Write 0x1E06 to WRAMADDR\r\n");
		return false;
	}
	if(!VS1053_SciRead(VS1053_REG_WRAM, &regVal))
	{
		printf("[VS1053] Failed to Read from WRAM\r\n");
		return false;
	}
	endFillByte = regVal & 0xFF;

	for(uint16_t i = 0; i < num; i++)
	{
		VS1053_SdiWrite(endFillByte);
	}
	return true;
}

/* Check DREQ pin */
bool VS1053_IsBusy()
{
	if (HAL_GPIO_ReadPin(VS1053_DREQ_PORT, VS1053_DREQ_PIN) == GPIO_PIN_SET) return false;  /* Ready */
	else return true;   /* Busy */
}

/* SCI Tx */
bool VS1053_SciWrite( uint8_t address, uint16_t input )
{
	uint8_t buffer[4];

	buffer[0] = VS1053_WRITE_CMD;
	buffer[1] = address;
	buffer[2] = input >> 8;			/* Input MSB */
	buffer[3] = input & 0x00FF;		/* Input LSB */

	while (HAL_GPIO_ReadPin(VS1053_DREQ_PORT, VS1053_DREQ_PIN) == GPIO_PIN_RESET);	/* Wait DREQ High */

	XCS_LOW;			/* XCS Low */
	if(HAL_SPI_Transmit(HSPI_VS1053, buffer, sizeof(buffer), 10) != HAL_OK)
	{
		printf("[VS1053] Failed to SPI Transmit\r\n");
		return false;
	}
	XCS_HIGH;			/* XCS High */

	while (HAL_GPIO_ReadPin(VS1053_DREQ_PORT, VS1053_DREQ_PIN) == GPIO_PIN_RESET);	/* Wait DREQ High */
	return true;
}

/* SCI TxRx */
bool VS1053_SciRead( uint8_t address, uint16_t *res)
{
	uint8_t dummy = 0xFF;
	uint8_t txBuffer[2];
	uint8_t rxBuffer[2];

	txBuffer[0] = VS1053_READ_CMD;
	txBuffer[1] = address;

	while(HAL_GPIO_ReadPin(VS1053_DREQ_PORT, VS1053_DREQ_PIN) == GPIO_PIN_RESET);	/* Wait DREQ High */

	XCS_LOW;        /* XCS Low */
	if(HAL_SPI_Transmit(HSPI_VS1053, txBuffer, sizeof(txBuffer), 10) != HAL_OK)
	{
		printf("[VS1053] Failed to SPI Transmit\r\n");
		return false;
	}
	if(HAL_SPI_TransmitReceive(HSPI_VS1053, &dummy, &rxBuffer[0], 1, 10) != HAL_OK)	{
		printf("[VS1053] Failed to SPI Receive\r\n");
		return false;
	}
	if(HAL_SPI_TransmitReceive(HSPI_VS1053, &dummy, &rxBuffer[1], 1, 10) != HAL_OK) 	{
		printf("[VS1053] Failed to SPI Receive\r\n");
		return false;
	}
	XCS_HIGH;       /* XCS High */

	*res = rxBuffer[0];     /* Received data */
	*res <<= 8;				/* MSB */
	*res |= rxBuffer[1];	/* LSB */

	while (HAL_GPIO_ReadPin(VS1053_DREQ_PORT, VS1053_DREQ_PIN) == GPIO_PIN_RESET);	/* Wait DREQ High */
	return true;
}

/* SDI Tx */
bool VS1053_SdiWrite( uint8_t input )
{
	while (HAL_GPIO_ReadPin(VS1053_DREQ_PORT, VS1053_DREQ_PIN) == GPIO_PIN_RESET);	/* Wait DREQ High */

	XDCS_LOW;			/* XDCS Low(SDI) */
	if(HAL_SPI_Transmit(HSPI_VS1053, &input, 1, 10) != HAL_OK)/* SPI Tx 1 byte */
	{
		printf("[VS1053] Failed to SPI Transmit\r\n");
		return false;
	}
	XDCS_HIGH;			/* XDCS High(SDI) */

	return true;
}
/**
 * @brief VS1053 SDI Write 32 bytes
 * @param input32 Pointer to the 32-byte data to be sent
 * @return true if successful, false otherwise
 * Prequisites:
 * - The VS1053 must be initialized and ready for communication.
 * - The DREQ pin must be high before calling this function.
 * - The SPI interface must be properly configured.
 */
/* SDI Tx 32 bytes */
bool VS1053_SdiWrite32( uint8_t *input32 )
{
	//while (HAL_GPIO_ReadPin(VS1053_DREQ_PORT, VS1053_DREQ_PIN) == GPIO_PIN_RESET);	/* Wait DREQ High */

	XDCS_LOW;			/* XDCS Low(SDI) */
	if(HAL_SPI_Transmit(HSPI_VS1053, input32, 32, 10) != HAL_OK) /* SPI Tx 32 bytes */
	{
		printf("[VS1053] Failed to SPI Transmit 32\r\n");
		return false;
	}
	XDCS_HIGH;			/* XDCS High(SDI) */

	return true;
}
/* SDI Tx 8 bytes */
bool VS1053_SdiWrite8( uint8_t *input8 )
{
	while (HAL_GPIO_ReadPin(VS1053_DREQ_PORT, VS1053_DREQ_PIN) == GPIO_PIN_RESET);	/* Wait DREQ High */

	XDCS_LOW;			/* XDCS Low(SDI) */
	if(HAL_SPI_Transmit(HSPI_VS1053, input8, 8, 10) != HAL_OK) /* SPI Tx 8 bytes */
	{
		printf("[VS1053] Failed to SPI Transmit 8\r\n");
		return false;
	}
	XDCS_HIGH;			/* XDCS High(SDI) */

	return true;
}
bool VS1053_I2S_En(void)
{
	// 5. Set Audio Format (AUDATA, Address 0x05)
	// 48 kHz, stereo
	if(!VS1053_SciWrite(VS1053_REG_AUDATA, 0xBB80))
	{
		printf("[VS1053] Failed to SciWrite  0xBB80 to AUDATA\r\n");
		return false;
	}
	if(!VS1053_SciWrite(VS1053_REG_WRAMADDR, 0xC017))
	{
		printf("[VS1053] Failed to SciWrite  0xC017 to WRAMADDR\r\n");
		return false;
	}
	uint16_t wramValue;
	if(!VS1053_SciRead(VS1053_REG_WRAM, &wramValue))
	{
		printf("[VS1053] Failed to SciRead from WRAM\r\n");
		return false;
	}
	printf("[VS1053] Original I2S Reg 0xC017: 0x%X\r\n",wramValue);
	wramValue |= 0xF0;  // Enable I2S
	if(!VS1053_SciWrite(VS1053_REG_WRAMADDR, 0xC017))
	{
		printf("[VS1053] Failed to SciWrite  0xC017 to WRAMADDR\r\n");
		return false;
	}
	if(!VS1053_SciWrite(VS1053_REG_WRAM, wramValue))
	{
		printf("[VS1053] Failed to SciWrite 0x%X to WRAM\r\n",wramValue);
		return false;
	}

	if(!VS1053_SciWrite(VS1053_REG_WRAMADDR, 0xC040))
	{
		printf("[VS1053] Failed to SciWrite  0xC040 to WRAMADDR\r\n");
		return false;
	}
	if(!VS1053_SciWrite(VS1053_REG_WRAM, 0x0C))
	{
		printf("[VS1053] Failed to SciWrite  0x0C to WRAM\r\n");
		return false;
	}

	// Read back to confirm
	if(!VS1053_SciWrite(VS1053_REG_WRAMADDR, 0xC017))
	{
		printf("[VS1053] Failed to SciWrite  0xC017 to WRAMADDR\r\n");
		return false;
	}
	uint16_t n_wramValue;
	if(!VS1053_SciRead(VS1053_REG_WRAM, &n_wramValue))
	{
		printf("[VS1053] Failed to SciRead from WRAM\r\n");
		return false;
	}
	printf("[VS1053] Original I2S Reg 0xC017: 0x%X\r\n",n_wramValue);
	return true;
}
void VS1053_SineTestStart(void)
{
	VS1053_Reset();
	uint8_t sine_activate[] = {0x53, 0xEF, 0x6E, 0x03, 0, 0, 0, 0};
	uint16_t mode = 0;
	VS1053_SciRead(VS1053_REG_MODE, &mode);
	VS1053_SciWrite(VS1053_REG_MODE, mode | VS1053_MODE_SM_TESTS);
	VS1053_SdiWrite8(sine_activate);
}
void VS1053_SineTestStop(void)
{
	uint8_t sine_deactivate[] = {0x45, 0x78, 0x69, 0x74, 0, 0, 0, 0};
	VS1053_SdiWrite8(sine_deactivate);
}
