/**
 *  @file encoder_driver.h
 *  @brief Header file for encoder driver and tick tracking functions.
 *  @author Andrew Carr and Kai De La Cruz
 *
 *  Created on: Jun 7, 2025
 */

#ifndef INC_ENCODER_DRIVER_H_
#define INC_ENCODER_DRIVER_H_

#include <stdint.h> // Allows use of standard integer types
#include "stm32f4xx_hal.h"  // or your STM32 family

/**
 * @brief Structure for tracking encoder position and timing.
 *
 * Holds values for the encoder's zero reference, accumulated tick count,
 * hardware configuration, and wrap-around handling.
 */
typedef struct
{
    uint32_t             zero;        /**< Timer value when "zeroed" */
    int32_t              ar;          /**< Auto-reload or max timer value */
    int32_t              ticks;       /**< Accumulated ticks */
    int32_t              last_ticks;  /**< Last read relative count */
    int32_t              range;       /**< Logical range of encoder */
    TIM_HandleTypeDef   *htim;        /**< Pointer to hardware timer handle */
} encoder_t;

/**
 * @brief Initializes the encoder by setting its zero reference.
 *
 * @param p_enc Pointer to an encoder_t structure.
 */
void setup_encoder(encoder_t *p_enc);

/**
 * @brief Updates the encoder tick count and applies wrap-around correction.
 *
 * @param p_enc Pointer to an encoder_t structure.
 */
void read_encoder(encoder_t *p_enc);

#endif /* INC_ENCODER_DRIVER_H_ */
