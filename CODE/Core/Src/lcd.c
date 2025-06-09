///
/// @file lcd.c
/// @brief Driver for 20x4 character LCD using I2C communication.
///
/// @details
/// @author Andrew Carr and Kai De La Cruz
///

#include "lcd.h"

#define LCD_BACKLIGHT 0x08
#define LCD_ENABLE    0x04
#define LCD_RW        0x00
#define LCD_RS        0x01

/**
 * @brief Sends a 4-bit nibble to the LCD.
 *
 * @param nibble Upper 4 bits of the byte
 * @param rs     Register select flag (0 = command, 1 = data)
 */
static void lcd_send_nibble(uint8_t nibble, uint8_t rs);

/**
 * @brief Sends a full byte to the LCD (as two 4-bit nibbles).
 *
 * @param byte The data byte to send
 * @param rs   Register select flag (0 = command, 1 = data)
 */
static void lcd_send_byte(uint8_t byte, uint8_t rs);

/**
 * @brief Sends a 4-bit nibble to the LCD with enable pulse.
 *
 * @param nibble The high nibble of the byte
 * @param rs     Register select flag (0 = command, 1 = data)
 */
static void lcd_send_nibble(uint8_t nibble, uint8_t rs) {
    uint8_t data = (nibble & 0xF0);
    if (rs) data |= LCD_RS;
    data |= LCD_BACKLIGHT;

    // Pulse enable
    uint8_t data_e = data | LCD_ENABLE;
    HAL_I2C_Master_Transmit(&hi2c1, LCD_I2C_ADDR,
                            &data_e, 1, HAL_MAX_DELAY);
    HAL_Delay(1);
    HAL_I2C_Master_Transmit(&hi2c1, LCD_I2C_ADDR,
                            &data, 1, HAL_MAX_DELAY);
    HAL_Delay(1);
}

/**
 * @brief Sends a full byte by splitting it into two nibbles.
 *
 * @param byte The byte to send
 * @param rs   Register select flag (0 = command, 1 = data)
 */
static void lcd_send_byte(uint8_t byte, uint8_t rs) {
    lcd_send_nibble(byte & 0xF0, rs);
    lcd_send_nibble((byte << 4) & 0xF0, rs);
}

/**
 * @brief Sends a command byte to the LCD.
 *
 * @param cmd The command byte
 */
void lcd_send_command(uint8_t cmd) {
    lcd_send_byte(cmd, 0);
    HAL_Delay(2);
}

/**
 * @brief Sends a data byte (character) to the LCD.
 *
 * @param data The character to display
 */
void lcd_send_data(uint8_t data) {
    lcd_send_byte(data, 1);
}

/**
 * @brief Forces the LCD backlight to stay on.
 */
void lcd_force_backlight_on(void) {
    uint8_t data = 0x00 | LCD_BACKLIGHT;
    HAL_I2C_Master_Transmit(&hi2c1, LCD_I2C_ADDR,
                            &data, 1, HAL_MAX_DELAY);
}

/**
 * @brief Initializes the LCD for 4-bit operation over I2C.
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
 * @brief Sets the LCD cursor position.
 *
 * @param row LCD row (0–3)
 * @param col LCD column (0–19)
 */
void lcd_set_cursor(uint8_t row, uint8_t col) {
    static const uint8_t row_offsets[] = {0x00, 0x40, 0x14, 0x54};
    if (row > 3) row = 3;
    lcd_send_command(0x80 | (col + row_offsets[row]));
}

/**
 * @brief Prints a null-terminated string to the LCD.
 *
 * @param str Pointer to the string to display
 */
void lcd_print(char *str) {
    while (*str) {
        lcd_send_data((uint8_t)*str++);
    }
}
