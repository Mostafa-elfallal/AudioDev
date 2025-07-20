#include "states.h"
#include "player.h"
#include <stdio.h>
#include "ui.h"
static char volumeText[5];

void volume_text_handle(sm_context_t *ctx)
{
	if(ctx->current_state == STATE_PLAYING)
	{
		sys_context_t *sys_ctx = (sys_context_t *)ctx->user_data;
		if(sys_ctx->is_muted)
		{
			sprintf(volumeText, "%d", 0);
		}
		else{
			sprintf(volumeText, "%d", sys_ctx->volume);
		}
		lv_label_set_text(ui_Label1, volumeText);
	}

}
void mute_button_handler(sm_context_t *ctx, sm_event_t event, void *event_data){
    printf("[COMMON] Mute button pressed\r\n");
    sys_context_t *sys_ctx = (sys_context_t *)ctx->user_data;
    sys_ctx->is_muted = !sys_ctx->is_muted; // Toggle mute state
    if (sys_ctx->is_muted) {
        printf("[COMMON] System muted\r\n");
        MP3_Mute();
    } else {
        printf("[COMMON] System unmuted\r\n");
        MP3_unMute();
    }
    volume_text_handle(ctx);
}
void volume_up_handler(sm_context_t *ctx, sm_event_t event, void *event_data){
	sys_context_t *sys_ctx = (sys_context_t *)ctx->user_data;
	sys_ctx->volume +=5 ;
	sys_ctx->volume = (sys_ctx->volume > 200) ? 200 : sys_ctx->volume;
    printf("[COMMON] Volume up\r\n");
    MP3_addVolume(5); // Increase volume by 5%
    volume_text_handle(ctx);
}
void volume_down_handler(sm_context_t *ctx, sm_event_t event, void *event_data){
	sys_context_t *sys_ctx = (sys_context_t *)ctx->user_data;
	sys_ctx->volume -=5 ;
	sys_ctx->volume = (sys_ctx->volume < 0) ? 0 : sys_ctx->volume;
    printf("[COMMON] Volume down\r\n");
    MP3_addVolume(-5); // decrease volume by 5%
    volume_text_handle(ctx);
}
