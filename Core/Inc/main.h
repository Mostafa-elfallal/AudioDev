/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.h
 * @brief          : Header for main.c file.
 *                   This file contains the common defines of the application.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define SRC_IFM0_Pin GPIO_PIN_2
#define SRC_IFM0_GPIO_Port GPIOE
#define SRC_IFM1_Pin GPIO_PIN_3
#define SRC_IFM1_GPIO_Port GPIOE
#define SRC_OWL0_Pin GPIO_PIN_4
#define SRC_OWL0_GPIO_Port GPIOE
#define SRC_OWL1_Pin GPIO_PIN_5
#define SRC_OWL1_GPIO_Port GPIOE
#define SRC_MUTE_Pin GPIO_PIN_6
#define SRC_MUTE_GPIO_Port GPIOE
#define SRC_BYPASS_Pin GPIO_PIN_13
#define SRC_BYPASS_GPIO_Port GPIOC
#define SRC_LGRP_Pin GPIO_PIN_14
#define SRC_LGRP_GPIO_Port GPIOC
#define SRC_MODE0_Pin GPIO_PIN_15
#define SRC_MODE0_GPIO_Port GPIOC
#define SRC_MODE1_Pin GPIO_PIN_0
#define SRC_MODE1_GPIO_Port GPIOF
#define SRC_MODE2_Pin GPIO_PIN_1
#define SRC_MODE2_GPIO_Port GPIOF
#define LCD_CS_Pin GPIO_PIN_6
#define LCD_CS_GPIO_Port GPIOF
#define USB_VBUS_EN_Pin GPIO_PIN_0
#define USB_VBUS_EN_GPIO_Port GPIOC
#define V_BATT_SENS_Pin GPIO_PIN_1
#define V_BATT_SENS_GPIO_Port GPIOA
#define MP3_CS_Pin GPIO_PIN_4
#define MP3_CS_GPIO_Port GPIOA
#define SP1_CLK_Pin GPIO_PIN_5
#define SP1_CLK_GPIO_Port GPIOA
#define SP1_MISO_Pin GPIO_PIN_6
#define SP1_MISO_GPIO_Port GPIOA
#define SPI1_MOSI_Pin GPIO_PIN_7
#define SPI1_MOSI_GPIO_Port GPIOA
#define MP3_XDSC_Pin GPIO_PIN_4
#define MP3_XDSC_GPIO_Port GPIOC
#define MP3_DREQ_Pin GPIO_PIN_5
#define MP3_DREQ_GPIO_Port GPIOC
#define MP3_RST_Pin GPIO_PIN_0
#define MP3_RST_GPIO_Port GPIOB
#define AMP_FAULT_Pin GPIO_PIN_11
#define AMP_FAULT_GPIO_Port GPIOF
#define AMP_MUTE_Pin GPIO_PIN_12
#define AMP_MUTE_GPIO_Port GPIOF
#define TXCO_ON_OFF_Pin GPIO_PIN_10
#define TXCO_ON_OFF_GPIO_Port GPIOE
#define LCD_CS2_Pin GPIO_PIN_11
#define LCD_CS2_GPIO_Port GPIOE
#define MUSES_LATCH_Pin GPIO_PIN_12
#define MUSES_LATCH_GPIO_Port GPIOB
#define MUSES_CLK_Pin GPIO_PIN_13
#define MUSES_CLK_GPIO_Port GPIOB
#define MUSES_DATA_Pin GPIO_PIN_15
#define MUSES_DATA_GPIO_Port GPIOB
#define LCD_RS_Pin GPIO_PIN_8
#define LCD_RS_GPIO_Port GPIOD
#define LCD_RST_Pin GPIO_PIN_9
#define LCD_RST_GPIO_Port GPIOD
#define BUTT_LED_CONT_Pin GPIO_PIN_12
#define BUTT_LED_CONT_GPIO_Port GPIOD
#define LCD_BL_Pin GPIO_PIN_13
#define LCD_BL_GPIO_Port GPIOD
#define FS_VBUS_Pin GPIO_PIN_9
#define FS_VBUS_GPIO_Port GPIOA
#define IRDA_Pin GPIO_PIN_10
#define IRDA_GPIO_Port GPIOA
#define USB_N_Pin GPIO_PIN_11
#define USB_N_GPIO_Port GPIOA
#define USB_P_Pin GPIO_PIN_12
#define USB_P_GPIO_Port GPIOA
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA
#define ST_I2S_LR_Pin GPIO_PIN_15
#define ST_I2S_LR_GPIO_Port GPIOA
#define SELECT_ENC_A_Pin GPIO_PIN_0
#define SELECT_ENC_A_GPIO_Port GPIOD
#define SELECT_ENC_A_EXTI_IRQn EXTI0_IRQn
#define SELECT_ENC_B_Pin GPIO_PIN_1
#define SELECT_ENC_B_GPIO_Port GPIOD
#define SELECT_ENC_B_EXTI_IRQn EXTI1_IRQn
#define SELECT_BUTT_Pin GPIO_PIN_2
#define SELECT_BUTT_GPIO_Port GPIOD
#define SELECT_BUTT_EXTI_IRQn EXTI2_IRQn
#define VOLU_ENC_A_Pin GPIO_PIN_3
#define VOLU_ENC_A_GPIO_Port GPIOD
#define VOLU_ENC_A_EXTI_IRQn EXTI3_IRQn
#define VOLU_ENC_B_Pin GPIO_PIN_4
#define VOLU_ENC_B_GPIO_Port GPIOD
#define VOLU_ENC_B_EXTI_IRQn EXTI4_IRQn
#define VOLU_MUTE_BUTT_Pin GPIO_PIN_5
#define VOLU_MUTE_BUTT_GPIO_Port GPIOD
#define VOLU_MUTE_BUTT_EXTI_IRQn EXTI9_5_IRQn
#define SATA_CFAST_CDI_Pin GPIO_PIN_6
#define SATA_CFAST_CDI_GPIO_Port GPIOD
#define ARD_Serial_RX_Pin GPIO_PIN_9
#define ARD_Serial_RX_GPIO_Port GPIOG
#define ARD_Serial_TX_Pin GPIO_PIN_14
#define ARD_Serial_TX_GPIO_Port GPIOG
#define NEXT_BUT_Pin GPIO_PIN_3
#define NEXT_BUT_GPIO_Port GPIOB
#define PREV_BUT_Pin GPIO_PIN_4
#define PREV_BUT_GPIO_Port GPIOB
#define BACK_BUT_Pin GPIO_PIN_5
#define BACK_BUT_GPIO_Port GPIOB
#define IRDA_TX_Pin GPIO_PIN_6
#define IRDA_TX_GPIO_Port GPIOB
#define SRC_RST_Pin GPIO_PIN_7
#define SRC_RST_GPIO_Port GPIOB
#define SRC_RDY_Pin GPIO_PIN_8
#define SRC_RDY_GPIO_Port GPIOB
#define SRC_OFM0_Pin GPIO_PIN_9
#define SRC_OFM0_GPIO_Port GPIOB
#define SRC_OFM1_Pin GPIO_PIN_0
#define SRC_OFM1_GPIO_Port GPIOE
#define SRC_IFM2_Pin GPIO_PIN_1
#define SRC_IFM2_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */
//#define USING_DEBUG_ONLY 1
#define ST7789_SPI_PORT hspi5
#define ST7789_RST_PORT LCD_RST_GPIO_Port
#define ST7789_RST_PIN  LCD_RST_Pin
#define ST7789_DC_PORT  LCD_RS_GPIO_Port
#define ST7789_DC_PIN   LCD_RS_Pin

#ifndef CFG_NO_CS
#define ST7789_CS_PORT  LCD_CS_GPIO_Port
#define ST7789_CS_PIN   LCD_CS_Pin
#endif
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
