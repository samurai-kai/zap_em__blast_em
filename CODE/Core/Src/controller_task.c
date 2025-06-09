/**
 *  @file controller_task.c
 *  @brief Implements controller task state machine for robot motor control.
 *  @author Kai De La Cruz
 *
 *
 *  Created on: Jun 3, 2025
 */

#include "motor_driver.h"
#include "controller_task.h"
#include "stm32f4xx_hal_tim.h"
#include "encoder_driver.h"
#include <stdio.h>

/**
 * @brief Function pointer type for controller state functions.
 */
typedef void (*StateFunc)(ControllerTask *);

/**
 * @brief Executes the current state of the controller task.
 *
 * Validates the current state index and calls the appropriate state
 * function from the task's function list.
 *
 * @param controller_task Pointer to the ControllerTask structure.
 */
void controller_task_run(ControllerTask *controller_task)
{
    if (controller_task->state >= 0 &&
        controller_task->state < controller_task->num_states)
    {
        controller_task->state_list[controller_task->state](controller_task);
    }
    else
    {
        while (1) {}
    }
}

/**
 * @brief Initializes the controller task (State 0).
 *
 * Enables the motor, records the potentiometer's zero point, and sets
 * the task to the next state.
 *
 * @param controller_task Pointer to the ControllerTask structure.
 */
void controller_task_state_0_init(ControllerTask *controller_task)
{
    controller_task->prev_error = 0;
    enable(controller_task->motor);

    // record ADC zero (center/home)
    controller_task->pot_zero = controller_task->adc_val;

    controller_task->state = 1;
}

/**
 * @brief Executes position control logic (State 1).
 *
 * Calculates desired position and error from encoder data and ADC
 * input, then applies control logic using a proportional-integral
 * method. Timing is measured using a hardware timer.
 *
 * @param controller_task Pointer to the ControllerTask structure.
 */
void controller_task_state_1_pos(ControllerTask *controller_task)
{
    int32_t des_pos = controller_task->dir *
                      (controller_task->adc_val -
                       controller_task->pot_zero);

    int32_t error = des_pos - controller_task->encoder->ticks;
    if (error > 100) error = 99;
    if (error < -100) error = -99;

    read_encoder(controller_task->encoder); // updates encoder ticks

    controller_task->current_time =
        __HAL_TIM_GET_COUNTER(controller_task->htim_dt);

    uint32_t dt_ticks =
        (controller_task->current_time >= controller_task->prev_time) ?
        (controller_task->current_time - controller_task->prev_time) :
        (0xFFFFFFFF - controller_task->prev_time +
         controller_task->current_time);

    if (dt_ticks == 0) return;

    int32_t delta_time = dt_ticks / 1000.0f;
    controller_task->integral_error += error * delta_time;

    int32_t control = controller_task->k_p * error +
                      controller_task->k_i *
                      controller_task->integral_error;

    set_duty(controller_task->motor, control);
}
