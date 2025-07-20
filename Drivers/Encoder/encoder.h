/*
 * encoder.h
 *
 *  Created on: May 21, 2025
 *      Author: mosta
 */

#ifndef ENCODER_ENCODER_H_
#define ENCODER_ENCODER_H_

#include "stm32f4xx_hal.h"
#include <stdint.h>

// Button structure for reusable button functionality
typedef struct {
    GPIO_TypeDef* GPIO_Port;
    uint16_t Pin;
    
    // Button state and debouncing
    uint8_t button_pressed;
    uint32_t last_button_tick;
    uint8_t last_button_state;
} Button_HandleTypeDef;

// Rotary encoder structure with embedded button
typedef struct {
    GPIO_TypeDef* GPIO_Port;
    uint16_t Pin_A;
    uint16_t Pin_B;
    
    volatile int32_t position;
    uint8_t prev_state;
    
    // Embedded button object
    Button_HandleTypeDef button;
} RotaryEncoder_HandleTypeDef;

// Button functions - can be used standalone or with encoder
void Button_Init(Button_HandleTypeDef* hbutton, GPIO_TypeDef* GPIO_Port, uint16_t Pin);
void Button_Update(Button_HandleTypeDef* hbutton);
uint8_t Button_WasPressed(Button_HandleTypeDef* hbutton);

// Rotary encoder functions
void RotaryEncoder_Init(RotaryEncoder_HandleTypeDef* henc, GPIO_TypeDef* GPIO_Port, 
                       uint16_t Pin_A, uint16_t Pin_B, uint16_t Pin_Button);
void RotaryEncoder_Update(RotaryEncoder_HandleTypeDef* henc);
void RotaryEncoder_ButtonUpdate(RotaryEncoder_HandleTypeDef* henc);
int32_t RotaryEncoder_GetPosition(RotaryEncoder_HandleTypeDef* henc);
void RotaryEncoder_Reset(RotaryEncoder_HandleTypeDef* henc);
uint8_t RotaryEncoder_WasButtonPressed(RotaryEncoder_HandleTypeDef* henc);

#endif /* ENCODER_ENCODER_H_ */