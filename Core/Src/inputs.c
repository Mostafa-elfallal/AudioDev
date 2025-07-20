#include "main.h"
#include "encoder.h"
#include "inputs.h"
#include "states.h"
RotaryEncoder_HandleTypeDef selectEnc;
RotaryEncoder_HandleTypeDef volumeEnc;
Button_HandleTypeDef nextButton;
Button_HandleTypeDef prevButton;
Button_HandleTypeDef backButton;
void inputs_init(void)
{
    // Initialize select encoder
    RotaryEncoder_Init(&selectEnc, SELECT_BUTT_GPIO_Port, SELECT_ENC_A_Pin, SELECT_ENC_B_Pin, SELECT_BUTT_Pin);
    
    // Initialize volume encoder
    RotaryEncoder_Init(&volumeEnc, VOLU_MUTE_BUTT_GPIO_Port, VOLU_ENC_A_Pin, VOLU_ENC_B_Pin, VOLU_MUTE_BUTT_Pin);
    
    // Initialize next button
    Button_Init(&nextButton, NEXT_BUT_GPIO_Port, NEXT_BUT_Pin);
    
    // Initialize previous button
    Button_Init(&prevButton, PREV_BUT_GPIO_Port, PREV_BUT_Pin);
    
    // Initialize back button
    Button_Init(&backButton, BACK_BUT_GPIO_Port, BACK_BUT_Pin);
}
void inputs_handle(void)
{
    // read volume encoder
    int32_t position = RotaryEncoder_GetPosition(&volumeEnc);
    uint8_t button_pressed = RotaryEncoder_WasButtonPressed(&volumeEnc);
    RotaryEncoder_Reset(&volumeEnc);
    if (position > 0)
    {
        sm_post_event(&sm_ctx, EVENT_VOLUME_UP, NULL); // Encoder turned right
    }
    else if (position < 0)
    {
        sm_post_event(&sm_ctx, EVENT_VOLUME_DOWN, NULL); // Encoder turned right
    }
    else if (button_pressed)
    {
        sm_post_event(&sm_ctx, EVENT_MUTE_BUTTON, NULL); // Button pressed
    }
    // read select encoder
    position = RotaryEncoder_GetPosition(&selectEnc);
    button_pressed = RotaryEncoder_WasButtonPressed(&selectEnc);
    RotaryEncoder_Reset(&selectEnc);
    if (position > 0)
    {
        sm_post_event(&sm_ctx, EVENT_SCROLL_UP, NULL); // Encoder turned right
    }
    else if (position < 0)
    {
        sm_post_event(&sm_ctx, EVENT_SCROLL_DOWN, NULL); // Encoder turned left
    }
    else if (button_pressed)
    {
        sm_post_event(&sm_ctx, EVENT_SELECT_BUTTON, NULL); // Button pressed
    }
    Button_Update(&nextButton);
    Button_Update(&prevButton);
    Button_Update(&backButton);
    // read the three buttons
    if (Button_WasPressed(&nextButton))
    {
        sm_post_event(&sm_ctx, EVENT_NEXT_BUTTON, NULL);
    }
    if (Button_WasPressed(&prevButton))
    {
        sm_post_event(&sm_ctx, EVENT_PREVIOUS_BUTTON, NULL);
    }
    if (Button_WasPressed(&backButton))
    {
        sm_post_event(&sm_ctx, EVENT_BACK_BUTTON, NULL);
    }
}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  switch (GPIO_Pin)
  {
    case SELECT_ENC_A_Pin:
    // case SELECT_ENC_B_Pin:
        RotaryEncoder_Update(&selectEnc);
        break;
    case SELECT_BUTT_Pin:
        RotaryEncoder_ButtonUpdate(&selectEnc);
        break;
    case VOLU_ENC_A_Pin:
    // case VOLU_ENC_B_Pin:
        RotaryEncoder_Update(&volumeEnc);
        break;
    case VOLU_MUTE_BUTT_Pin:
        RotaryEncoder_ButtonUpdate(&volumeEnc);
        break;
    default:
        break;
  }
}
