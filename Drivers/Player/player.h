/*
 * player.h
 *
 *  Created on: May 20, 2025
 *      Author: mosta
 */

#ifndef VS1053_PLAYER_H_
#define VS1053_PLAYER_H_

#include <stdbool.h>
#include <stm32f4xx.h>

/* Functions */
bool MP3_Init(void);

bool MP3_Play(const char *filename);
void MP3_Stop(void);
void MP3_Pause(void);
void MP3_Resume(void);
void MP3_Feeder(void);
void MP3_Mute(void);
void MP3_unMute(void);
/* Flags */
bool MP3_isPlaying(void);
bool MP3_isMuted(void);
/* Volume Control */
/*
 * ourPercentage from 0 to 200
 * 0 means mute
 * 100 equals no attenuation
 * above 100 equals having gain
 * */
void MP3_setVolume(uint16_t vPercentage);
uint16_t MP3_getVolume();
void MP3_addVolume(int16_t delta);
#endif /* VS1053_PLAYER_H_ */
