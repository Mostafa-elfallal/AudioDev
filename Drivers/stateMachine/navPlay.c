#include "states.h"
#include <stdio.h>
#include "screens.h"
void navigating_playing_entry(sm_context_t *ctx, sm_event_t event, void *event_data){
    printf("[NAV_PLAY] Entering Navigating Play State\n");
    sys_context_t *sys_ctx = (sys_context_t *)ctx->user_data;
    // Initialize the file browser with the current directory
    file_browser_init(sys_ctx);
}

void navigating_playing_exit(sm_context_t *ctx, sm_event_t event, void *event_data){
    printf("[NAV_PLAY] Exiting Navigating Play State\n");
    file_item_t *selected_item = (file_item_t *)event_data;
    if (selected_item) {
        printf("[NAV_PLAY] Selected item: %s, is_directory: %d\n", selected_item->name, selected_item->is_directory);
    }
    
    
    else {
        printf("[NAV_PLAY] No item selected\n");
    }
}
void navigating_playing_event(sm_context_t *ctx, sm_event_t event, void *event_data){
        sys_context_t *sys_ctx = (sys_context_t *)ctx->user_data;
    switch (event) {
        case EVENT_MUTE_BUTTON:
            printf("[NAV_PLAY] Mute button pressed\n");
            mute_button_handler(ctx, event, event_data);
            break;
        case EVENT_VOLUME_UP:
            printf("[NAV_PLAY] Volume up\n");
            volume_up_handler(ctx, event, event_data);
            break;
        case EVENT_VOLUME_DOWN:
            printf("[NAV_PLAY] Volume down\n");
            volume_down_handler(ctx, event, event_data);
            break;
        case EVENT_SCROLL_UP:
            printf("[NAV_PLAY] Scroll up\n");
            // Handle scroll up logic here
            file_browser_send_key(LV_KEY_UP);
            break;
        case EVENT_SCROLL_DOWN:
            printf("[NAV_PLAY] Scroll down\n");
            // Handle scroll down logic here
            file_browser_send_key(LV_KEY_DOWN);
            break;
        case EVENT_SELECT_BUTTON:
            printf("[NAV_PLAY] Select button pressed\n");
            // Handle select button logic here
            file_browser_send_key(LV_KEY_ENTER);
            break;
        default:
            printf("[NAV_PLAY] Unknown event: %d\n", event);
            break;
    }
}   
