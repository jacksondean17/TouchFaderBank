  /******************************************************************************
  * @file           : IS31FL3236A_led_driver.h
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
#ifndef __IS31FL3236A_LED_DRIVER_H
#define __IS31FL3236A_LED_DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "stm32f0xx_hal.h"


#define DRIVER0_I2C_ADDR (0x3C << 1)
#define DRIVER1_I2C_ADDR (0x3D << 1)
#define DRIVER2_I2C_ADDR (0x3E << 1)
#define DRIVER3_I2C_ADDR (0x3F << 1)

#define I2CBUF_SIZE (256)

// 0 to 255
#define LED_PWM_BRIGHTNESS (127)


HAL_StatusTypeDef _LED_Driver_Write(I2C_HandleTypeDef *hi2c, uint8_t addr, uint8_t* buffer, uint8_t n_bytes);
void LED_Driver_Write(uint8_t addr, uint8_t* buffer, uint8_t n_bytes);
void InitDriverHAL(I2C_HandleTypeDef *hi2c);
void InitLEDDriver(uint8_t id);
void SetLEDPWMs(uint8_t id);
void SetLEDControl(uint8_t id, uint8_t led_id, uint8_t state);
void SetLEDs(uint8_t id, uint8_t *buf, uint8_t n_bytes);





#ifdef __cplusplus
}
#endif

#endif /* __IS31FL3236A_LED_DRIVER_H */
