/**
 *  @file photoresistor_task.h
 *  @brief Header for photoresistor-based hit detection task logic.
 *  @author Andrew Carr
 *
 *  Created on: Jun 3, 2025
 */

#ifndef INC_PHOTORESISTOR_TASK_H_
#define INC_PHOTORESISTOR_TASK_H_

#include <stdint.h> // Allows use of standard integer types
#include "stm32f4xx_hal.h"  // or your STM32 family

/**
 * @brief Forward declaration of PhotoresistorTask structure.
 */
typedef struct PhotoresistorTask PhotoresistorTask;

/**
 * @brief Forward declaration of GameTask structure.
 */
typedef struct GameTask GameTask;

/**
 * @brief Function pointer type for photoresistor state functions.
 */
typedef void (*photoresistor_fcn_t)(
    PhotoresistorTask *photoresistor_task);

/**
 * @brief Structure for tracking photoresistor task state and config.
 *
 * Holds ADC value, thresholds, state index, and a list of function
 * pointers for state execution.
 */
struct PhotoresistorTask
{
    int32_t                 state;         /**< Current state index */
    int32_t                 num_states;    /**< Number of valid states */
    int32_t                 hit_flag;      /**< Set when hit is detected */
    int32_t                 adc_val;       /**< Current ADC reading */
    int32_t                 thresh;        /**< Threshold for hit detect */
    int32_t                 zero;          /**< Zero offset for calibration */
    photoresistor_fcn_t     state_list[];  /**< Array of state functions */
};

/**
 * @brief Initializes the photoresistor task (state 0).
 *
 * @param photoresistor_task Pointer to the task structure.
 */
void photoresistor_task_state_0_init(
    PhotoresistorTask *photoresistor_task);

/**
 * @brief Executes the current task state function.
 *
 * @param photoresistor_task Pointer to the task structure.
 */
void photoresistor_task_run(
    PhotoresistorTask *photoresistor_task);

/**
 * @brief Checks for a light-based hit (state 1).
 *
 * @param photoresistor_task Pointer to the task structure.
 */
void photoresistor_task_state_1_look(
    PhotoresistorTask *photoresistor_task);

/**
 * @brief Waits for hit flag to clear (state 2).
 *
 * @param photoresistor_task Pointer to the task structure.
 */
void photoresistor_task_state_2_hit(
    PhotoresistorTask *photoresistor_task);

#endif /* INC_PHOTORESISTOR_TASK_H_ */
