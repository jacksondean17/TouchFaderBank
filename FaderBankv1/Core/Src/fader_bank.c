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
	
	void InitFaderBank(void) {
		for (int i = 0; i < FB_TOTAL_TRACKS; i++) {
			FaderBank[i].pos = 0;
		}
	}
	
	
