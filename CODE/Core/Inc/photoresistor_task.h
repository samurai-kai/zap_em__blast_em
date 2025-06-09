/**
 *  @file photoresistor_task.h
 *  @brief Header for photoresistor task used in hit detection logic.
 *  @author Andrew Carr
 *
 *  Created on: Jun 3, 2025
 */

#ifndef INC_PHOTORESISTOR_TASK_H_
#define INC_PHOTORESISTOR_TASK_H_

#include <stdint.h>
#include "stm32f4xx_hal.h"

/**
 * @brief Forward declaration of PhotoresistorTask structure.
 */
typedef struct PhotoresistorTask PhotoresistorTask;

/**
 * @brief Forward declaration of GameTask structure.
 */
typedef struct GameTask GameTask;

/**
 * @brief Function pointer type for photoresistor task states.
 *
 * Each state function takes a pointer to a PhotoresistorTask object.
 */
typedef void (*photoresistor_fcn_t)(
    PhotoresistorTask *photoresistor_task);

/**
 * @brief Structure for photoresistor task configuration and state.
 *
 * Contains ADC values, hit detection threshold, and a state machine
 * for light-triggered gameplay events.
 */
struct PhotoresistorTask
{
    int32_t              state;        /**< Current task state */
    int32_t              num_states;   /**< Total number of states */
    int32_t              hit_flag;     /**< 1 if light was detected */
    int32_t              adc_val;      /**< Current ADC reading */
    int32_t              thresh;       /**< Hit threshold above zero */
    int32_t              zero;         /**< Baseline ADC reference */
    photoresistor_fcn_t  state_list[]; /**< List of state functions */
};

/**
 * @brief Initializes the photoresistor task (state 0).
 *
 * @param photoresistor_task Pointer to PhotoresistorTask instance.
 */
void photoresistor_task_state_0_init(
    PhotoresistorTask *photoresistor_task);

/**
 * @brief Executes the current state of the task.
 *
 * @param photoresistor_task Pointer to PhotoresistorTask instance.
 */
void photoresistor_task_run(
    PhotoresistorTask *photoresistor_task);

/**
 * @brief Performs light detection logic (state 1).
 *
 * @param photoresistor_task Pointer to PhotoresistorTask instance.
 */
void photoresistor_task_state_1_look(
    PhotoresistorTask *photoresistor_task);

/**
 * @brief Waits for hit flag to be cleared (state 2).
 *
 * @param photoresistor_task Pointer to PhotoresistorTask instance.
 */
void photoresistor_task_state_2_hit(
    PhotoresistorTask *photoresistor_task);

#endif /* INC_PHOTORESISTOR_TASK_H_ */
