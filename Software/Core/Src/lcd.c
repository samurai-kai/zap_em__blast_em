///
/// @file lcd.c
/// @brief LCD driver for 20x4 I2C display. Adapted from SunFounder example.
/// @date May 22, 2025
/// @author
/// Andrew Carr and Kai De La Cruz
///
/// @note Modified for STM32 HAL and tailored for 20x4 LCD output.
///

#include "lcd.h"

#define LCD_BACKLIGHT 0x08
#define LCD_ENABLE    0x04
#define LCD_RW        0x00
#define LCD_RS        0x01

/**
 * @brief Sends a 4-bit nibble to the LCD with optional RS control.
 *
 * @param nibble Upper 4 bits of byte
 * @param rs     Register Select (0 = command, 1 = data)
 */
static void lcd_send_nibble(uint8_t nibble, uint8_t rs);

/**
 * @brief Sends a full byte to the LCD by splitting into nibbles.
 *
 * @param byte Full byte to send
 * @param rs   Register Select (0 = command, 1 = data)
 */
static void lcd_send_byte(uint8_t byte, uint8_t rs);

static void lcd_send_nibble(uint8_t nibble, uint8_t rs) {
    uint8_t data = (nibble & 0xF0);
    if (rs) data |= LCD_RS;
    data |= LCD_BACKLIGHT;

    // Pulse enable
    uint8_t data_e = data | LCD_ENABLE;
    HAL_I2C_Master_Transmit(&hi2c1, LCD_I2C_ADDR, &data_e, 1, HAL_MAX_DELAY);
    HAL_Delay(1);
    HAL_I2C_Master_Transmit(&hi2c1, LCD_I2C_ADDR, &data, 1, HAL_MAX_DELAY);
    HAL_Delay(1);
}

static void lcd_send_byte(uint8_t byte, uint8_t rs) {
    lcd_send_nibble(byte & 0xF0, rs);
    lcd_send_nibble((byte << 4) & 0xF0, rs);
}

/**
 * @brief Sends a command byte to the LCD.
 *
 * @param cmd Command byte
 */
void lcd_send_command(uint8_t cmd) {
    lcd_send_byte(cmd, 0);
    HAL_Delay(2);
}

/**
 * @brief Sends a data byte (character) to the LCD.
 *
 * @param data Data byte
 */
void lcd_send_data(uint8_t data) {
    lcd_send_byte(data, 1);
}

/**
 * @brief Forces the LCD backlight to turn on.
 */
void lcd_force_backlight_on(void) {
    uint8_t data = 0x00 | LCD_BACKLIGHT;
    HAL_I2C_Master_Transmit(&hi2c1, LCD_I2C_ADDR, &data, 1, HAL_MAX_DELAY);
}

/**
 * @brief Initializes the LCD in 4-bit mode.
 */
void lcd_init(void) {
    HAL_Delay(50); // Wait for power to stabilize

    // Set to 4-bit mode
    lcd_send_nibble(0x30, 0); HAL_Delay(5);
    lcd_send_nibble(0x30, 0); HAL_Delay(1);
    lcd_send_nibble(0x30, 0); HAL_Delay(1);
    lcd_send_nibble(0x20, 0); HAL_Delay(1); // 4-bit mode

    lcd_send_command(0x28); // Function set: 4-bit, 2-line, 5x8 dots
    lcd_send_command(0x08); // Display off
    lcd_send_command(0x01); // Clear display
    HAL_Delay(2);
    lcd_send_command(0x06); // Entry mode set
    lcd_send_command(0x0C); // Display on, cursor off
    lcd_force_backlight_on();
}

/**
 * @brief Clears the LCD display.
 */
void lcd_clear(void) {
    lcd_send_command(0x01);
    HAL_Delay(2);
}

/**
 * @brief Sets the LCD cursor to the specified position.
 *
 * @param row Row index (0–3)
 * @param col Column index (0–19)
 */
void lcd_set_cursor(uint8_t row, uint8_t col) {
    static const uint8_t row_offsets[] = {0x00, 0x40, 0x14, 0x54};
    if (row > 3) row = 3;
    lcd_send_command(0x80 | (col + row_offsets[row]));
}

/**
 * @brief Prints a string to the LCD display.
 *
 * @param str Null-terminated character string
 */
void lcd_print(char *str) {
    while (*str) {
        lcd_send_data((uint8_t)*str++);
    }
}
