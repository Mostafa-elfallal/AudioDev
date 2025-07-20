/*
 * encoder.c
 *
 *  Created on: May 21, 2025
 *      Author: mosta
 */

#include "encoder.h"

#define DEBOUNCE_DELAY_MS 50

static const int8_t transition_table[16] = {
     0, -1,  1,  0,
     1,  0,  0, -1,
    -1,  0,  0,  1,
     0,  1, -1,  0
};

static inline uint8_t read_encoder_state(RotaryEncoder_HandleTypeDef* henc) {
    return ((HAL_GPIO_ReadPin(henc->GPIO_Port, henc->Pin_A) << 1) |
             HAL_GPIO_ReadPin(henc->GPIO_Port, henc->Pin_B));
}

// ============== BUTTON FUNCTIONS ==============

void Button_Init(Button_HandleTypeDef* hbutton, GPIO_TypeDef* GPIO_Port, uint16_t Pin) {
    hbutton->GPIO_Port = GPIO_Port;
    hbutton->Pin = Pin;
    hbutton->button_pressed = 0;
    hbutton->last_button_state = HAL_GPIO_ReadPin(hbutton->GPIO_Port, hbutton->Pin);
    hbutton->last_button_tick = HAL_GetTick();
}

void Button_Update(Button_HandleTypeDef* hbutton) {
    uint8_t current_state = HAL_GPIO_ReadPin(hbutton->GPIO_Port, hbutton->Pin);
    uint32_t now = HAL_GetTick();

    if (current_state != hbutton->last_button_state) {
        // Potential state change detected
        if ((now - hbutton->last_button_tick) >= DEBOUNCE_DELAY_MS) {
            // Stable state after debounce period
            if (current_state == GPIO_PIN_RESET) {
                hbutton->button_pressed = 1; // Falling edge (press detected)
            }
            hbutton->last_button_state = current_state;
        }
        hbutton->last_button_tick = now;
    }
}

uint8_t Button_WasPressed(Button_HandleTypeDef* hbutton) {
    uint8_t pressed = hbutton->button_pressed;
    hbutton->button_pressed = 0;
    return pressed;
}

// ============== ROTARY ENCODER FUNCTIONS ==============

void RotaryEncoder_Init(RotaryEncoder_HandleTypeDef* henc, GPIO_TypeDef* GPIO_Port, 
                       uint16_t Pin_A, uint16_t Pin_B, uint16_t Pin_Button) {
    // Initialize encoder pins
    henc->GPIO_Port = GPIO_Port;
    henc->Pin_A = Pin_A;
    henc->Pin_B = Pin_B;
    
    // Initialize encoder state
    henc->prev_state = read_encoder_state(henc);
    henc->position = 0;
    
    // Initialize embedded button
    Button_Init(&henc->button, GPIO_Port, Pin_Button);
}

void RotaryEncoder_Update(RotaryEncoder_HandleTypeDef* henc) {
    uint8_t new_state = read_encoder_state(henc);
    uint8_t index = (henc->prev_state << 2) | new_state;
    henc->position += transition_table[index];
    henc->prev_state = new_state;
}

void RotaryEncoder_ButtonUpdate(RotaryEncoder_HandleTypeDef* henc) {
    Button_Update(&henc->button);
}

int32_t RotaryEncoder_GetPosition(RotaryEncoder_HandleTypeDef* henc) {
    return henc->position;
}

void RotaryEncoder_Reset(RotaryEncoder_HandleTypeDef* henc) {
    henc->position = 0;
}

uint8_t RotaryEncoder_WasButtonPressed(RotaryEncoder_HandleTypeDef* henc) {
    return Button_WasPressed(&henc->button);
}