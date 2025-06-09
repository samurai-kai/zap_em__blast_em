/**
 *  @file shoot_task.h
 *  @brief Header file for laser shooting and servo shielding task logic.
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
typedef struct ShootTask ShootTask;

/**
 * @brief Function pointer type for ShootTask state functions.
 *
 * Each function accepts a pointer to a ShootTask instance.
 */
typedef void (*shoot_fcn_t)(ShootTask *shoot_task);

/**
 * @brief Structure for shoot task configuration and state.
 *
 * Holds GPIO, PWM, and state data for managing a shooting mechanism
 * using a laser and servo motor. Intended for use in a state machine.
 */
struct ShootTask
{
    int32_t             state;          /**< Current state index */
    int32_t             num_states;     /**< Total number of states */
    int32_t             button;         /**< Button state flag */
    TIM_HandleTypeDef  *servo_tim;      /**< Timer for servo PWM */
    uint32_t            channel;        /**< PWM channel for servo */
    int32_t             shield_val;     /**< PWM value for shielded pos */
    int32_t             unshield_val;   /**< PWM value for unshielded pos */
    uint32_t            laser_gpio;     /**< GPIO pin for laser control */
    shoot_fcn_t         state_list[];   /**< State function pointer array */
};

/**
 * @brief Initializes the shoot task (state 0).
 *
 * @param shoot_task Pointer to the ShootTask structure.
 */
void shoot_task_state_0_init(ShootTask *shoot_task);

/**
 * @brief Executes the current state in the shoot task.
 *
 * @param shoot_task Pointer to the ShootTask structure.
 */
void shoot_task_run(ShootTask *shoot_task);

/**
 * @brief Waits for button press to trigger shoot action (state 1).
 *
 * @param shoot_task Pointer to the ShootTask structure.
 */
void shoot_task_state_1_wait(ShootTask *shoot_task);

/**
 * @brief Lowers shield before firing laser (state 2).
 *
 * @param shoot_task Pointer to the ShootTask structure.
 */
void shoot_task_state_2_unshield(ShootTask *shoot_task);

/**
 * @brief Fires laser and resets to wait state (state 3).
 *
 * @param shoot_task Pointer to the ShootTask structure.
 */
void shoot_task_state_3_shoot(ShootTask *shoot_task);

#endif /* INC_SHOOT_TASK_H_ */
