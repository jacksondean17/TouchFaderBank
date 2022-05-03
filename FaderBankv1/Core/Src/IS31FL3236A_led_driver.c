/**
  ******************************************************************************
  * @file           : IS31FL3236A_led_driver.c
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
	
#include "IS31FL3236A_led_driver.h"
	
I2C_HandleTypeDef *_hi2c;
uint8_t i2cbuf[I2CBUF_SIZE];
uint8_t UPDATE_REG_DATA[] = {0x25, 0x00};
const uint8_t DRIVER_ADDRS[] = {DRIVER0_I2C_ADDR, DRIVER1_I2C_ADDR, DRIVER2_I2C_ADDR, DRIVER3_I2C_ADDR};

	
HAL_StatusTypeDef _LED_Driver_Write(I2C_HandleTypeDef *hi2c, uint8_t addr, uint8_t* buffer, uint8_t n_bytes) {
	return HAL_I2C_Master_Transmit(hi2c, addr, buffer, n_bytes, 10000);
	
}

void LED_Driver_Write(uint8_t addr, uint8_t* buffer, uint8_t n_bytes) {
	_LED_Driver_Write(_hi2c, addr, buffer, n_bytes);
		// Write PWM Update Register ...
	_LED_Driver_Write(_hi2c, addr, UPDATE_REG_DATA, 2);
}

void InitDriverHAL(I2C_HandleTypeDef *hi2c) {
	_hi2c = hi2c;
}

void InitLEDDriver(uint8_t id) {
	uint8_t addr = DRIVER_ADDRS[id];
		// Disable software shutdown mode ...
	i2cbuf[0] = 0x00; // Address of Shutdown Register
	i2cbuf[1] = 0x01; // Setting for normal operation
	LED_Driver_Write(addr, i2cbuf, 2);
	
	i2cbuf[0] = 0x28;
	for (int i = 1; i <= 36; i++) {
		i2cbuf[i] = 0;
	}
	SetLEDs(id, i2cbuf, 37);
}

void SetLEDPWMs(uint8_t id) {
	i2cbuf[0] = 0x01; // Address of OUT1 PWM Register
	for (int i = 1; i <= 37; i++) {
		i2cbuf[i] = LED_PWM_BRIGHTNESS;
	}
	uint8_t addr = DRIVER_ADDRS[id];
	LED_Driver_Write(addr, i2cbuf, 38);

}

void SetLEDControl(uint8_t id, uint8_t led_id, uint8_t state) {
	uint8_t addr = DRIVER_ADDRS[id];
	i2cbuf[0] = 0x27 + led_id; // LED control register
	i2cbuf[1] = state;
	LED_Driver_Write(addr, i2cbuf, 2);
}

void SetLEDs(uint8_t id, uint8_t *buf, uint8_t n_bytes) {
	uint8_t addr = DRIVER_ADDRS[id];
	LED_Driver_Write(addr, buf, n_bytes);
}
