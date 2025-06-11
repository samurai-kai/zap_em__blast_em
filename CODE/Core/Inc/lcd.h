/**
// *  @file lcd.h
// *  @brief Header file for I2C 20x4 LCD control functions.
// *  @author Andrew Carr and Kai De La Cruz
// *
// *  Created on: May 22, 2025
// *
// */

#ifndef LCD_H
#define LCD_H

#include "stm32f4xx_hal.h"

/// Adjust to match your LCD I2C address (typically 0x27 or 0x3F)
#define LCD_I2C_ADDR (0x27 << 1)

/// External handle to use your I2C peripheral
extern I2C_HandleTypeDef hi2c1;

/**
 * @brief Initializes the LCD in 4-bit I2C mode.
 */
void lcd_init(void);

/**
 * @brief Clears the LCD display.
 */
void lcd_clear(void);

/**
 * @brief Sets the LCD cursor to a specific row and column.
 *
 * @param row Row index (0–3)
 * @param col Column index (0–19)
 */
void lcd_set_cursor(uint8_t row, uint8_t col);

/**
 * @brief Prints a null-terminated string to the LCD.
 *
 * @param str Pointer to the string to print
 */
void lcd_print(char *str);

/**
 * @brief Sends a command byte to the LCD.
 *
 * @param cmd Command to send
 */
void lcd_send_command(uint8_t cmd);

/**
 * @brief Sends a data byte (character) to the LCD.
 *
 * @param data Character to display
 */
void lcd_send_data(uint8_t data);

/**
 * @brief Forces the LCD backlight to remain on.
 */
void lcd_force_backlight_on(void);

#endif // LCD_H
