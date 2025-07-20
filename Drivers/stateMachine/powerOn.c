#include "states.h"
#include "st7789_lvgl.h"
#include "logger.h"
#include "inputs.h"
#include <stdio.h>

extern UART_HandleTypeDef huart2;
void power_on_entry(sm_context_t *ctx, sm_event_t event, void *event_data){
    logger_init(&huart2);
    inputs_init();
    printf("[POWER_ON] Entering Power On State\r\n");
    lv_init();
    lv_port_disp_init();
    lv_obj_clean(lv_scr_act());
   assert_param(MP3_Init() == true);
    sys_context_t *sys_ctx = (sys_context_t *)ctx->user_data;
   MP3_setVolume(sys_ctx->volume);
    sm_post_event(ctx, EVENT_INITIALIZED, NULL);
}
void power_on_exit(sm_context_t *ctx, sm_event_t event, void *event_data){
    printf("[POWER_ON] Exiting Power On State\r\n");
}
void power_on_event(sm_context_t *ctx, sm_event_t event, void *event_data){
    printf("[POWER_ON] Handling event %d\r\n", event);
}
