/*
 * lcd.h
 *
 *  Created on: May 22, 2025
 *      Author: andrewcarr
 */

#ifndef INC_LCD_H_
#define INC_LCD_H_


#include "stm32f4xx_hal.h"  // or your STM32 family
#include <stdint.h>

void lcd_init(I2C_HandleTypeDef *hi2c);
void lcd_clear(void);
void lcd_write(uint8_t x, uint8_t y, const char *str);
void lcd_send_command(uint8_t cmd);
void lcd_send_data(uint8_t data);


#endif /* INC_LCD_H_ */
