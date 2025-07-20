#ifndef __STATESMACHINE_STATES_H
#define __STATESMACHINE_STATES_H
#include "stateMachine.h"
#include "stdint.h"
typedef struct
{
    char name[64];
    bool is_directory;
} file_item_t;

typedef struct
{
    char path[256];
    file_item_t *items;
    int item_count;
} directory_t;

typedef enum {
    OPTION_PLAYBACK_REPEAT = 0,
    OPTION_PLAYBACK_ALL,
    OPTION_PLAYBACK_SHUFFLE,
    OPTION_PLAYBACK_NONE,
}sys_option_t;

typedef struct{
    directory_t current_directory; // Current directory being browsed
    sys_option_t playback_option; // Current playback option
    uint8_t volume; // Current volume level (0-200)
    bool is_muted; // Mute state
}sys_context_t;

typedef enum {
    STATE_POWER_ON = 0,
    STATE_SPLASH,
    STATE_USB_CHECK,
    STATE_USB_MOUNT,
    STATE_USB_UNMOUNT,
    STATE_NAVIGATING_IDLE,
    STATE_NAVIGATING_PLAYING,
    STATE_PLAYING
} sys_state_t;

typedef enum {
    EVENT_NONE = 0, // used for invoking state functions without an event to consume time
    EVENT_INITIALIZED,
	EVENT_UI_READY,
    EVENT_USB_MOUNTED,
    EVENT_USB_UNMOUNTED,
    EVENT_SCROLL_UP,
    EVENT_SCROLL_DOWN,
    EVENT_SELECT_BUTTON,
    EVENT_SELECT_ITEM,
    EVENT_VOLUME_UP,
    EVENT_VOLUME_DOWN,
    EVENT_MUTE_BUTTON,
    EVENT_BACK_BUTTON,
    EVENT_NEXT_BUTTON,
    EVENT_PREVIOUS_BUTTON,
    EVENT_STOP_PLAYBACK,
    EVENT_FINISHED_PLAYBACK
} sys_event_t;

// prototypes for state functions from states.c
// entries for each state
void power_on_entry(sm_context_t *ctx, sm_event_t event, void *event_data);
void splash_entry(sm_context_t *ctx, sm_event_t event, void *event_data);
void usb_check_entry(sm_context_t *ctx, sm_event_t event, void *event_data);
void usb_mount_entry(sm_context_t *ctx, sm_event_t event, void *event_data);
void usb_unmount_entry(sm_context_t *ctx, sm_event_t event, void *event_data);
void navigating_idle_entry(sm_context_t *ctx, sm_event_t event, void *event_data);
void navigating_playing_entry(sm_context_t *ctx, sm_event_t event, void *event_data);
void playing_entry(sm_context_t *ctx, sm_event_t event, void *event_data);
// exits for each state
void power_on_exit(sm_context_t *ctx, sm_event_t event, void *event_data);
void splash_exit(sm_context_t *ctx, sm_event_t event, void *event_data);
void usb_check_exit(sm_context_t *ctx, sm_event_t event, void *event_data);
void usb_mount_exit(sm_context_t *ctx, sm_event_t event, void *event_data);
void usb_unmount_exit(sm_context_t *ctx, sm_event_t event, void *event_data);
void navigating_idle_exit(sm_context_t *ctx, sm_event_t event, void *event_data);
void navigating_playing_exit(sm_context_t *ctx, sm_event_t event, void *event_data);
void playing_exit(sm_context_t *ctx, sm_event_t event, void *event_data);
// events for each state
void power_on_event(sm_context_t *ctx, sm_event_t event, void *event_data);
void splash_event(sm_context_t *ctx, sm_event_t event, void *event_data);
void usb_check_event(sm_context_t *ctx, sm_event_t event, void *event_data);
void navigating_idle_event(sm_context_t *ctx, sm_event_t event, void *event_data);
void navigating_playing_event(sm_context_t *ctx, sm_event_t event, void *event_data);
void playing_event(sm_context_t *ctx, sm_event_t event, void *event_data);
// common handlers for events
void volume_down_handler(sm_context_t *ctx, sm_event_t event, void *event_data);
void volume_up_handler(sm_context_t *ctx, sm_event_t event, void *event_data);
void mute_button_handler(sm_context_t *ctx, sm_event_t event, void *event_data);
void volume_text_handle(sm_context_t *ctx);

sm_result_t states_init(sm_context_t *sm_ctx, sys_context_t *sys_ctx);

extern sm_context_t sm_ctx; // Global state machine context
extern sys_context_t sys_ctx; // Global system context
#endif // __STATESMACHINE_STATES_H
