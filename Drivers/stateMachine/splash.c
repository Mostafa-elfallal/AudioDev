#include "states.h"
#include "ui.h"
#include <stdio.h>
#include "main.h"
static uint32_t Splash_timer = 0;
#define SPLASH_TIMEOUT 3000 // 3 seconds
void splash_entry(sm_context_t *ctx, sm_event_t event, void *event_data)
{
    printf("[SPLASH] Entering Splash State\r\n");
    ui_init();

    Splash_timer = HAL_GetTick();
    sm_post_event(ctx, EVENT_NONE, NULL);
}

void splash_exit(sm_context_t *ctx, sm_event_t event, void *event_data){
    printf("[SPLASH] Exiting Splash State\r\n");
//    ui_Splash_screen_destroy();
}
void splash_event(sm_context_t *ctx, sm_event_t event, void *event_data)
{
    printf("[SPLASH] Handling event %d\r\n", event);
    switch (event)
    {
        case EVENT_NONE:
           if(HAL_GetTick() - Splash_timer >= SPLASH_TIMEOUT) {
                printf("[SPLASH] Splash timeout reached, transitioning to USB_CHECK\r\n");
                sm_post_event(ctx, EVENT_UI_READY, NULL);
            }
            else {
                sm_post_event(ctx, EVENT_NONE, NULL);
            }
            break;
        default:
            printf("[SPLASH] Unknown event %d\r\n", event);
            break;
    }
}
