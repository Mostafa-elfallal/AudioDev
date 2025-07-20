
#include "states.h"
#include "screens.h"
#include "File_Handling.h"
#include "player.h"
void usb_check_entry(sm_context_t *ctx, sm_event_t event, void *event_data){
    printf("[USB_CHECK] Entering USB Check State\r\n");
    // is it already mounted?
    if (USB_IsReady()) {
        // USB is mounted, transition to navigating idle state
        sm_post_event(ctx, EVENT_USB_MOUNTED, NULL);
    } else {
        // USB is not mounted, show USB check screen
        show_usb_check_screen();
    }
}
void usb_check_exit(sm_context_t *ctx, sm_event_t event, void *event_data){
    printf("[USB_CHECK] Exiting USB Check State\r\n");
}

void usb_check_event(sm_context_t *ctx, sm_event_t event, void *event_data){
    
    switch (event) {
        case EVENT_MUTE_BUTTON:
            printf("[USB_CHECK] Mute Button Pressed\r\n");
            mute_button_handler(ctx, event, event_data);
            break;
        case EVENT_VOLUME_UP:
            printf("[USB_CHECK] Volume up\r\n");
            volume_up_handler(ctx, event, event_data);
            break;
        case EVENT_VOLUME_DOWN:
            printf("[USB_CHECK] Volume down\r\n");
            volume_down_handler(ctx, event, event_data);
            break;
        default:
            printf("[USB_CHECK] Unknown event: %d\r\n", event);
            break;
    }
}