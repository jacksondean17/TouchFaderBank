/**
  ******************************************************************************
  * @file           : fader_bank.h
  * @brief          : Functions and structs for working with faders
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 Alef Audio
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
	
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FADER_BANK_H
#define __FADER_BANK_H

#ifdef __cplusplus
extern "C" {
#endif

#include "IS31FL3236A_led_driver.h"

#define RELATIVE_FADER_MOVEMENT
	
#define FB_TOTAL_TRACKS (4)

#define FADER_POSITION(track) (FaderBank[track].pos)

	
/** Position of a Fader Track sensor.
  */
typedef struct {
		uint8_t 		pos;
		uint8_t 		rel_starting_pos;
		uint8_t 		abs_starting_pos;
} 
FB_Track_T;


void InitFaderBank(I2C_HandleTypeDef *hi2c);

void SetFaderTrack(uint8_t track_id, uint8_t pos);

int AdjustFaderTrack(uint8_t track_id, uint8_t pos);

void SetFaderTrackStartPos(uint8_t track_id, uint8_t pos);


extern FB_Track_T FaderBank[];

#ifdef __cplusplus
}
#endif

#endif /* __FADER_BANK_H */
