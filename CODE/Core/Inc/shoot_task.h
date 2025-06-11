/**
 *  @file shoot_task.h
 *  @brief Header file for shoot task state machine and control logic.
 *  @author Andrew Carr
 *
 *  Created on: Jun 3, 2025
 */

#ifndef INC_SHOOT_TASK_H_
#define INC_SHOOT_TASK_H_

#include <stdint.h> // Allows use of standard integer types
#include "stm32f4xx_hal.h"  // or your STM32 family

/**
 * @brief Forward declaration of ShootTask structure.
 */
typedef struct ShootTask ShootTask; // forward declaration

/**
 * @brief Function pointer type for shoot task state functions.
 */
typedef void (*shoot_fcn_t)(ShootTask *shoot_task);

/**
 * @brief Structure for shoot task configuration and state tracking.
 *
 * Contains information for laser control, servo control, and button input
 * processing. Implements a state machine via function pointer list.
 */
struct ShootTask
{
    int32_t              state;         /**< Current state index */
    int32_t              num_states;    /**< Total number of states */
    int32_t              button;        /**< Button press flag */
    TIM_HandleTypeDef   *servo_tim;     /**< Timer for servo PWM */
    uint32_t             channel;       /**< PWM channel for servo */
    int32_t              shield_val;    /**< PWM value for shield position */
    int32_t              unshield_val;  /**< PWM value for unshielded pos */
    uint32_t             laser_gpio;    /**< GPIO pin for laser control */
    shoot_fcn_t          state_list[];  /**< Array of state functions */
};

/**
 * @brief Initializes the shoot task (state 0).
 *
 * @param shoot_task Pointer to ShootTask structure.
 */
void shoot_task_state_0_init(ShootTask *shoot_task);

/**
 * @brief Executes the current shoot task state.
 *
 * @param shoot_task Pointer to ShootTask structure.
 */
void shoot_task_run(ShootTask *shoot_task);

/**
 * @brief Waits for user input to activate shoot sequence (state 1).
 *
 * @param shoot_task Pointer to ShootTask structure.
 */
void shoot_task_state_1_wait(ShootTask *shoot_task);

/**
 * @brief Lowers servo shield prior to firing laser (state 2).
 *
 * @param shoot_task Pointer to ShootTask structure.
 */
void shoot_task_state_2_unshield(ShootTask *shoot_task);

/**
 * @brief Fires the laser and returns to wait state after release (state 3).
 *
 * @param shoot_task Pointer to ShootTask structure.
 */
void shoot_task_state_3_shoot(ShootTask *shoot_task);

#endif /* INC_SHOOT_TASK_H_ */
