/*
 * logger.h
 *
 *  Created on: May 25, 2025
 *      Author: mosta
 */

#ifndef LOGGER_LOGGER_H_
#define LOGGER_LOGGER_H_
#include "stm32f4xx_hal.h"
typedef enum {
    PRINTF_TO_UART,
    PRINTF_TO_USB
} PrintfOutputDest_t;

void logger_init(UART_HandleTypeDef * uart);
void logger_change(PrintfOutputDest_t);
#endif /* LOGGER_LOGGER_H_ */
