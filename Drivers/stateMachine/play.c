#include "states.h"
#include <stdio.h>
#include "player.h"
#include "ui.h"
// we must save the selected file path here as global variable to be used if we are invoked from navigating playing state
char selected_file_full_path[256] = {0};
char selected_file_name[64] = {0};
static uint32_t Play_timer = 0;
#define PLAY_TIMEOUT 1000
void playing_entry(sm_context_t *ctx, sm_event_t event, void *event_data){
    // we have two cases here 
    // 1 - we have file to play as user selected a file (event_data is not NULL) , the path is in ctx->user_data and file name is in event_data , should be concatenated
    // 2 - we have no file as user pressed back button from navigating state
    sys_context_t *sys_ctx = (sys_context_t *)ctx->user_data;
    if (event_data) {
        // Case 1: Play the selected file
        file_item_t *selected_file = (file_item_t *)event_data;
        // take care if the path is root or not
        snprintf(selected_file_name, sizeof(selected_file_name), "%s", selected_file->name);
        if (strcmp(sys_ctx->current_directory.path, "/") == 0) {
            snprintf(selected_file_full_path, sizeof(selected_file_full_path), "/%s", selected_file->name);
        } else {
            // Concatenate the current directory path with the selected file name
            snprintf(selected_file_full_path, sizeof(selected_file_full_path), "%s/%s", sys_ctx->current_directory.path, selected_file->name);
        }
        printf("[PLAYING] Playing file: %s\n", selected_file_full_path);

        Play_timer = HAL_GetTick();
        sm_post_event(ctx, EVENT_NONE, NULL);
    } else {
        // Case 2: No file to play, just return to navigating state
        printf("[PLAYING] No file selected\n");
        // sm_post_event(ctx, EVENT_BACK_BUTTON, NULL);
    }
    // invoke the UI with changing the value of the label to the selected file name
    volume_text_handle(ctx);
    lv_label_set_text(ui_Music_Title1, selected_file_name);
    ui_Music_Player_screen_init();
    lv_disp_load_scr(ui_Music_Player);
}

void playing_exit(sm_context_t *ctx, sm_event_t event, void *event_data){
    printf("[PLAYING] Exiting Playing State\n");
    // Here we can stop the playback if needed
    // MP3_Stop(); // Assuming MP3_Stop is a function that stops the playback
}
void playing_event(sm_context_t *ctx, sm_event_t event, void *event_data){
    printf("[PLAYING] Handling event: %d\n", event);
    sys_context_t *sys_ctx = (sys_context_t *)ctx->user_data;
    switch (event) {
    	case EVENT_NONE:
		   if(HAL_GetTick() - Play_timer >= PLAY_TIMEOUT) {
				printf("[Play] Player timeout reached\r\n");
				sm_post_event(ctx, EVENT_UI_READY, NULL);
			}
			else {
				sm_post_event(ctx, EVENT_NONE, NULL);
			}
		   break;
        case EVENT_FINISHED_PLAYBACK:
            printf("[PLAYING] finished the playback\n");
            // TODO: check the playback option and handle it
            sm_post_event(ctx, EVENT_STOP_PLAYBACK, NULL); // Post back event to return to navigating state
            break;
        case EVENT_USB_UNMOUNTED:
            printf("[PLAYING] USB unmounted, returning to USB check state\n");
            MP3_Stop(); // Stop playback
            sm_post_event(ctx, EVENT_USB_UNMOUNTED, NULL); // Post USB unmount event
            break;
        case EVENT_SELECT_BUTTON:
            printf("[PLAYING] Select item event received, but already in playing state\n");
            // No action needed as we are already playing
            if(MP3_isPlaying()) {
                printf("[PLAYING] Already playing: %s\n", selected_file_full_path);
                MP3_Pause();
            } else {
                printf("[PLAYING] Not playing, should not happen here\n");
                MP3_Resume();
            }
            break;
        case EVENT_NEXT_BUTTON:
            printf("[PLAYING] Next button pressed, should handle next track logic\n");
            // Implement logic to play the next track
            break;
        case EVENT_PREVIOUS_BUTTON:
            printf("[PLAYING] Previous button pressed, should handle previous track logic\n");
            // Implement logic to play the previous track
            break;
        case EVENT_MUTE_BUTTON:
            printf("[PLAYING] Mute button pressed, toggling mute state\n");
            mute_button_handler(ctx,event,event_data); // Assuming mute_button_handler is a function that toggles mute state
            break;
        case EVENT_VOLUME_UP:
            printf("[PLAYING] Volume up event received\n");
            volume_up_handler(ctx, event, event_data); // Assuming volume_up_handler is a function that increases volume
            break;
        case EVENT_VOLUME_DOWN:
            printf("[PLAYING] Volume down event received\n");
            volume_down_handler(ctx, event, event_data); // Assuming volume_down_handler is a function that decreases volume
            break;
        case EVENT_UI_READY:
        	printf("[PLAYING] UI READY event received\n");
        	MP3_Play(selected_file_full_path); // Assuming MP3_Play is a function that plays the file
        	break;
        default:
            printf("[PLAYING] Unknown event: %d\n", event);
            break;
    }
}
