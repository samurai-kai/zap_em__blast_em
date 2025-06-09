/**
 *  @file controller_task.h
 *  @brief Header file for controller task logic in Zap'Em Blast'Em Robots.
 *  @author Kai De La Cruz
 *
 *  Created on: Jun 3, 2025
 */

#ifndef INC_CONTROLLER_TASK_H_
#define INC_CONTROLLER_TASK_H_

#include "motor_driver.h"
#include <stdlib.h>
#include <stdio.h>
#include "stm32f4xx_hal.h"
#include "encoder_driver.h"
#include <stdint.h> // Allows use of standard integer types

/**
 * @brief Forward declaration of ControllerTask structure.
 */
typedef struct ControllerTask ControllerTask; // forward declaration

/**
 * @brief Function pointer type for controller task state functions.
 *
 * Each function takes a pointer to a ControllerTask struct.
 */
typedef void (*controller_fcn_t)(ControllerTask *controller_task);

/**
 * @brief Structure that defines the controller task.
 *
 * Contains all configuration, timing, control, and peripheral references
 * needed to manage a motor control task using state functions.
 */
struct ControllerTask
{
    int32_t             dir;              /**< Direction of movement */
    int32_t             state;            /**< Current state index */
    int32_t             num_states;       /**< Total number of states */
    uint32_t            chan1;            /**< PWM channel 1 */
    uint32_t            chan2;            /**< PWM channel 2 */
    int32_t             pot_zero;         /**< Zero offset from ADC pot */
    int32_t             des_pos;          /**< Desired position in ticks */
    uint32_t            prev_time;        /**< Previous time reading */
    uint32_t            current_time;     /**< Current time reading */
    uint32_t            prev_ticks;       /**< Last encoder tick value */
    float               k_p;              /**< Proportional gain */
    float               k_i;              /**< Integral gain */
    uint32_t            adc_val;          /**< Current ADC value */
    int32_t             integral_error;   /**< Integrated error term */
    TIM_HandleTypeDef   *htim_encoder;    /**< Timer for encoder ticks */
    TIM_HandleTypeDef   *htim_dt;         /**< Timer for delta time calc */
    ADC_HandleTypeDef   *hadc;            /**< ADC handle for potentiometer */
    motor_t             *motor;           /**< Pointer to motor object */
    encoder_t           *encoder;         /**< Pointer to encoder object */

    float               prev_error;       /**< Previous PID error */

    controller_fcn_t    state_list[];     /**< Function pointer list for states */
};

/**
 * @brief Initializes the controller task (State 0).
 *
 * @param controller_task Pointer to the ControllerTask instance.
 */
void controller_task_state_0_init(ControllerTask *controller_task);

/**
 * @brief Executes the current state of the controller task.
 *
 * @param controller_task Pointer to the ControllerTask instance.
 */
void controller_task_run(ControllerTask *controller_task);

/**
 * @brief Executes the position control logic (State 1).
 *
 * @param controller_task Pointer to the ControllerTask instance.
 */
void controller_task_state_1_pos(ControllerTask *controller_task);

#endif /* INC_CONTROLLER_TASK_H_ */
