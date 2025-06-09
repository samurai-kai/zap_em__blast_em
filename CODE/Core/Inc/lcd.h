///
/// @file lcd.h
/// @brief Header file for 20x4 I2C LCD driver functions.
/// @author Andrew Carr and Kai De La Cruz
/// @date May 22, 2025
///

#ifndef LCD_H
#define LCD_H

#include "stm32f4xx_hal.h"

/// @brief I2C address of the LCD (shifted left for HAL).
#define LCD_I2C_ADDR (0x27 << 1)

/// @brief External I2C handle used for communication with the LCD.
extern I2C_HandleTypeDef hi2c1;

/**
 * @brief Initializes the LCD for 4-bit I2C communication.
 */
void lcd_init(void);

/**
 * @brief Clears the LCD display.
 */
void lcd_clear(void);

/**
 * @brief Sets the cursor to a specific row and column.
 *
 * @param row Row index (0–3)
 * @param col Column index (0–19)
 */
void lcd_set_cursor(uint8_t row, uint8_t col);

/**
 * @brief Prints a null-terminated string to the LCD.
 *
 * @param str Pointer to the character string to print
 */
void lcd_print(char *str);

/**
 * @brief Sends a command byte to the LCD.
 *
 * @param cmd Command byte
 */
void lcd_send_command(uint8_t cmd);

/**
 * @brief Sends a data byte (character) to the LCD.
 *
 * @param data Data byte
 */
void lcd_send_data(uint8_t data);

/**
 * @brief Forces the LCD backlight to stay on.
 */
void lcd_force_backlight_on(void);

#endif // LCD_H
