/**
 *  @file encoder_driver.h
 *  @brief Header file for encoder structure and read/setup functions.
 *  @author Andrew Carr and Kai De La Cruz
 *
 *  Created on: Jun 7, 2025
 */

#ifndef INC_ENCODER_DRIVER_H_
#define INC_ENCODER_DRIVER_H_

#include <stdint.h> // Allows use of standard integer types
#include "stm32f4xx_hal.h"  // or your STM32 family

/**
 * @brief Structure to hold encoder task configuration and state.
 *
 * This struct includes timer values, encoder tick accumulation,
 * and wrap-around handling through the auto-reload value.
 */
typedef struct
{
    uint32_t             zero;        /**< Timer value when "zeroed" */
    int32_t              ar;          /**< Auto-reload or max timer value */
    int32_t              ticks;       /**< Accumulated ticks */
    int32_t              last_ticks;  /**< Last read relative count */
    int32_t              range;       /**< Logical range of encoder */
    TIM_HandleTypeDef   *htim;        /**< Timer handle */
} encoder_t;

/**
 * @brief Initializes the encoder by setting its zero reference.
 *
 * @param p_enc Pointer to encoder_t instance to initialize.
 */
void setup_encoder(encoder_t *p_enc);

/**
 * @brief Reads encoder ticks and applies wrap-around correction.
 *
 * Updates cumulative tick count and stores the latest tick snapshot.
 *
 * @param p_enc Pointer to encoder_t instance to read from.
 */
void read_encoder(encoder_t *p_enc);

#endif /* INC_ENCODER_DRIVER_H_ */
