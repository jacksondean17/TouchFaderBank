/**
  ******************************************************************************
  * @file           : fader_bank.c
  * @brief          : Functions for working with faders
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
	
	#include "fader_bank.h"
	
	FB_Track_T FaderBank[FB_TOTAL_TRACKS];
	
	
	#define LED_BUF_SIZE (33)
	uint8_t led_buf[LED_BUF_SIZE];
	
	void InitFaderBank(I2C_HandleTypeDef *hi2c) {
		InitDriverHAL(hi2c);
		
		for (int i = 0; i < FB_TOTAL_TRACKS; i++) {
			FaderBank[i].pos = 0;
			InitLEDDriver(i);
			SetFaderTrack(i, 0);
		}
	}
	
	void SetFaderTrack(uint8_t track_id, uint8_t pos) {
		FaderBank[track_id].pos = pos;
		int level = pos >> 3; // convert from range [0, 127] to [0, 16]
		level = level+1; // to allow full range of led bar. Would like to always have bottom bar lit to show position.
		
		led_buf[0] = 0x26; // TODO move this logic to LED Driver code
		for (int i = 1; i <= level; i++) {
			led_buf[17-i] = 0x01;
			led_buf[16+i] = 0x01;
		}
		for (int i = level+1; i <= 16; i++) {
			led_buf[17-i] = 0x00;
			led_buf[16+i] = 0x00;
		}
		
		SetLEDs(track_id, led_buf, LED_BUF_SIZE);
	}
	
	int AdjustFaderTrack(uint8_t track_id, uint8_t pos) {
		int change = pos - FaderBank[track_id].rel_starting_pos;
		
		int16_t new_pos = FaderBank[track_id].abs_starting_pos + change;
		if (new_pos < 0) new_pos = 0;
		if (new_pos > 127) new_pos = 127;
	
		SetFaderTrack(track_id, new_pos);
		return change;
	}
	
	void SetFaderTrackStartPos(uint8_t track_id, uint8_t pos) {
		FaderBank[track_id].rel_starting_pos = pos;
		FaderBank[track_id].abs_starting_pos = FaderBank[track_id].pos;
	}
	
	
