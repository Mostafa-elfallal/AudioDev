#include "states.h"
#include "File_Handling.h"

#include <stdio.h>

void usb_mount_entry(sm_context_t *ctx, sm_event_t event, void *event_data)
{
    printf("[USB_MOUNT] Mounting USB...\n");
    if(Mount_USB() != FR_OK) {
        printf("[USB_MOUNT] Failed to mount USB\n");
        // If mount fails, we can post an unmount event
        sm_post_event(ctx, EVENT_USB_UNMOUNTED, NULL);
    }
    else{
        printf("[USB_MOUNT] USB mounted successfully\n");
        sm_post_event(ctx, EVENT_USB_MOUNTED, NULL);
    }
}
void usb_unmount_entry(sm_context_t *ctx, sm_event_t event, void *event_data)
{
    printf("[USB_UNMOUNT] Unmounting USB...\n");
    if(Unmount_USB() == FR_OK) {
        printf("[USB_UNMOUNT] USB unmounted successfully\n");
        sm_post_event(ctx, EVENT_USB_UNMOUNTED, NULL);
    }
    else {
        printf("[USB_UNMOUNT] Failed to unmount USB\n");
    }
}

void usb_mount_exit(sm_context_t *ctx, sm_event_t event, void *event_data)
{
    printf("[USB_MOUNT] Exiting USB Mount State\n");
    // Here we can perform any cleanup if needed
}
void usb_unmount_exit(sm_context_t *ctx, sm_event_t event, void *event_data)
{
    printf("[USB_UNMOUNT] Exiting USB Unmount State\n");
    // Here we can perform any cleanup if needed
}