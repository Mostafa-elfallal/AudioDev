/*
 * logger.c
 *
 *  Created on: May 25, 2025
 *      Author: mosta
 */
#include "logger.h"
#include "stdlib.h"
#include <string.h>
#include "File_Handling.h"
static volatile PrintfOutputDest_t printf_dest = PRINTF_TO_UART;
static UART_HandleTypeDef* UART = NULL;
void logger_init(UART_HandleTypeDef * uart)
{
	UART = uart;
	printf_dest = PRINTF_TO_UART;
}
void logger_change(PrintfOutputDest_t d)
{
	printf_dest = d;
}

int _write(int file, char *ptr, int len) {
    if (printf_dest == PRINTF_TO_USB) {
        // Optional: buffer protection and truncation
        char logBuffer[256];
        if (len >= sizeof(logBuffer)) len = sizeof(logBuffer) - 1;
        memcpy(logBuffer, ptr, len);
        logBuffer[len] = '\0';

        Update_File("/logs.txt", logBuffer);
        return len;

    } else if (printf_dest == PRINTF_TO_UART) {
    	if(UART)
    		HAL_UART_Transmit(UART, (uint8_t*)ptr, len, HAL_MAX_DELAY);
        return len;

    } else {
        // Unknown target
        return -1;
    }
}
