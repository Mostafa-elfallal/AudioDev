#include "states.h"

// State definitions
static const sm_state_def_t audio_player_states[] = {
    {
        .state_id = STATE_POWER_ON,
        .on_entry = power_on_entry,
        .on_exit = power_on_exit,
        .on_event = power_on_event,
        .parent_state = SM_STATE_NONE,
        .name = "POWER_ON"
    },
    {
        .state_id = STATE_SPLASH,
        .on_entry = splash_entry,
        .on_exit = splash_exit,
        .on_event = splash_event,
        .parent_state = SM_STATE_NONE,
        .name = "SPLASH"
    },
    {
        .state_id = STATE_USB_MOUNT,
        .on_entry = usb_mount_entry,
        .on_exit = usb_mount_exit,
        .on_event = NULL, 
        .parent_state = SM_STATE_NONE,
        .name = "USB_MOUNT"
    },
        {
        .state_id = STATE_USB_UNMOUNT,
        .on_entry = usb_mount_entry,
        .on_exit = usb_mount_exit,
        .on_event = NULL, 
        .parent_state = SM_STATE_NONE,
        .name = "USB_UNMOUNT"
    },
    {
        .state_id = STATE_USB_CHECK,
        .on_entry = usb_check_entry,
        .on_exit = usb_check_exit,
        .on_event = usb_check_event, 
        .parent_state = SM_STATE_NONE,
        .name = "USB_CHECK"
    },
    {
        .state_id = STATE_NAVIGATING_IDLE,
        .on_entry = navigating_idle_entry,
        .on_exit = navigating_idle_exit,
        .on_event = navigating_idle_event,
        .parent_state = SM_STATE_NONE,
        .name = "NAVIGATING_IDLE"
    },
    {
        .state_id = STATE_NAVIGATING_PLAYING,
        .on_entry = navigating_playing_entry,
        .on_exit = navigating_playing_exit,
        .on_event = navigating_playing_event,
        .parent_state = SM_STATE_NONE,
        .name = "NAVIGATING_PLAYING"
    },
    {
        .state_id = STATE_PLAYING,
        .on_entry = playing_entry,
        .on_exit = playing_exit,
        .on_event = playing_event,
        .parent_state = SM_STATE_NONE,
        .name = "PLAYING"
    }
};

// =============================================================================
// TRANSITION TABLE
// =============================================================================

static const sm_transition_t audio_player_transitions[] = {
    // From POWER_ON
    {
        .from_state = STATE_POWER_ON,
        .event = EVENT_INITIALIZED,
        .to_state = STATE_SPLASH,
        .guard = NULL,
        .action = NULL
    },
    
    // From SPLASH (automatic transition after display)
    {
        .from_state = STATE_SPLASH,
        .event = EVENT_UI_READY,
        .to_state = STATE_USB_CHECK,
        .guard = NULL,
        .action = NULL
    },
    
    // From USB_CHECK
    {
        .from_state = STATE_USB_CHECK,
        .event = EVENT_USB_MOUNTED,
        .to_state = STATE_USB_MOUNT,
        .guard = NULL,
        .action = NULL
    },
    {
        .from_state = STATE_USB_CHECK,
        .event = EVENT_USB_UNMOUNTED,
        .to_state = STATE_USB_UNMOUNT, 
        .guard = NULL,
        .action = NULL
    },
    // From USB_MOUNT
    {
        .from_state = STATE_USB_MOUNT,
        .event = EVENT_USB_UNMOUNTED,
        .to_state = STATE_USB_UNMOUNT, 
        .guard = NULL,
        .action = NULL
    },
    {
        .from_state = STATE_USB_MOUNT,
        .event = EVENT_USB_MOUNTED, // reuse for USB mount confirmation
        .to_state = STATE_NAVIGATING_IDLE,
        .guard = NULL,
        .action = NULL
    },
    // From USB_UNMOUNT
    {
        .from_state = STATE_USB_UNMOUNT,
        .event = EVENT_USB_MOUNTED, // reuse for USB mount confirmation
        .to_state = STATE_USB_MOUNT,
        .guard = NULL,
        .action = NULL
    },
    {
        .from_state = STATE_USB_UNMOUNT,
        .event = EVENT_USB_UNMOUNTED, // reuse for USB unmount confirmation
        .to_state = STATE_USB_CHECK,
        .guard = NULL,
        .action = NULL
    },
    
    // From NAVIGATING_IDLE
    {
        .from_state = STATE_NAVIGATING_IDLE,
        .event = EVENT_SELECT_ITEM,
        .to_state = STATE_PLAYING,
        .guard = NULL, 
        .action = NULL
    },
    {
        .from_state = STATE_NAVIGATING_IDLE,
        .event = EVENT_USB_UNMOUNTED,
        .to_state = STATE_USB_CHECK,
        .guard = NULL,
        .action = NULL
    },
    
    // From NAVIGATING_PLAYING
    {
        .from_state = STATE_NAVIGATING_PLAYING,
        .event = EVENT_SELECT_ITEM,
        .to_state = STATE_PLAYING,
        .guard = NULL,
        .action = NULL
    },
    {
        .from_state = STATE_NAVIGATING_PLAYING,
        .event = EVENT_BACK_BUTTON,
        .to_state = STATE_PLAYING,
        .guard = NULL,
        .action = NULL
    },
    {
        .from_state = STATE_NAVIGATING_PLAYING,
        .event = EVENT_USB_UNMOUNTED,
        .to_state = STATE_USB_CHECK,
        .guard = NULL,
        .action = NULL
    },
    
    // From PLAYING
    {
        .from_state = STATE_PLAYING,
        .event = EVENT_BACK_BUTTON,
        .to_state = STATE_NAVIGATING_PLAYING,
        .guard = NULL,
        .action = NULL
    },
    {
        .from_state = STATE_PLAYING,
        .event = EVENT_STOP_PLAYBACK,
        .to_state = STATE_NAVIGATING_IDLE,
        .guard = NULL,
        .action = NULL
    },
    {
        .from_state = STATE_PLAYING,
        .event = EVENT_USB_UNMOUNTED,
        .to_state = STATE_USB_CHECK,
        .guard = NULL,
        .action = NULL
    }
};

  sm_context_t sm_ctx;
  sys_context_t sys_ctx = {
      .current_directory = {
          .path = "/",
          .items = NULL,
          .item_count = 0
      },
        .playback_option = OPTION_PLAYBACK_NONE, // Default playback option
        .volume = 80, // Default volume level
        .is_muted = false // Default mute state
  };
// =============================================================================
// Initialize the state machine
sm_result_t states_init(sm_context_t *sm_ctx, sys_context_t *sys_ctx) {
    // Initialize the state machine context
    sm_result_t res = sm_init(sm_ctx, audio_player_states, 
            sizeof(audio_player_states) / sizeof(audio_player_states[0]),
            audio_player_transitions, 
            sizeof(audio_player_transitions) / sizeof(audio_player_transitions[0]),
            STATE_POWER_ON, // Initial state
            sys_ctx);
    return res;
}
