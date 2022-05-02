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
	
#define FB_TOTAL_TRACKS (4)

#define FADER_POSITION(track) (FaderBank[track].pos)
#define SET_FADER_POSITION(track, newPos) (FaderBank[track].pos = newPos)

	
/** Position of a Fader Track sensor.
  */
typedef struct {
		int 		pos;
} 
FB_Track_T;


void InitFaderBank(void);


extern FB_Track_T FaderBank[];

#ifdef __cplusplus
}
#endif

#endif /* __FADER_BANK_H */
