/**
 *  @file shoot_task.c
 *  @brief Task logic for laser and servo control during shooting state.
 *  @author Andrew Carr
 *
 *  Created on: Jun 3, 2025
 */

#include "shoot_task.h"
#include <stdio.h>

// game plan is to create two tasks, one for red and one for blue

/**
 * @brief Executes the current shoot task state function.
 *
 * Checks that the current state index is valid and calls the
 * corresponding state handler. Falls into an infinite loop if the
 * state index is invalid.
 *
 * @param shoot_task Pointer to ShootTask structure.
 */
void shoot_task_run(ShootTask *shoot_task)
{
    if (shoot_task->state >= 0 &&
        shoot_task->state < shoot_task->num_states)
    {
        shoot_task->state_list[shoot_task->state](shoot_task);
    }
    else
    {
        while(1) {}
    }
}

// servo -- 300 at shield

/**
 * @brief Initializes the shoot task (state 0).
 *
 * Initializes laser and servo to unshielded state and moves to state 1.
 *
 * @param shoot_task Pointer to ShootTask structure.
 */
void shoot_task_state_0_init(ShootTask *shoot_task)
{
    shoot_task->state = 1;
    HAL_GPIO_WritePin(GPIOB, shoot_task->laser_gpio, GPIO_PIN_SET);
    __HAL_TIM_SET_COMPARE(shoot_task->servo_tim,
                          shoot_task->channel,
                          shoot_task->unshield_val);
}

/**
 * @brief Waits for button press to initiate action (state 1).
 *
 * Keeps laser on and servo unshielded. Advances to state 2 when button
 * is pressed.
 *
 * @param shoot_task Pointer to ShootTask structure.
 */
void shoot_task_state_1_wait(ShootTask *shoot_task)
{
    HAL_GPIO_WritePin(GPIOB, shoot_task->laser_gpio, GPIO_PIN_SET);
    __HAL_TIM_SET_COMPARE(shoot_task->servo_tim,
                          shoot_task->channel,
                          shoot_task->unshield_val);
    if (shoot_task->button == 1) {
        shoot_task->state = 2;
    }
}

/**
 * @brief Lowers the servo shield before firing (state 2).
 *
 * After lowering shield, transitions to state 3. Also checks for early
 * button release to return to state 1.
 *
 * @param shoot_task Pointer to ShootTask structure.
 */
void shoot_task_state_2_unshield(ShootTask *shoot_task)
{
    __HAL_TIM_SET_COMPARE(shoot_task->servo_tim,
                          shoot_task->channel,
                          shoot_task->shield_val);
    if (1) { // add delay using some ticks and tune it
        shoot_task->state = 3;
    }
    if (shoot_task->button == 0) {
        shoot_task->state = 1;
    }
}

/**
 * @brief Fires the laser and resets state if button is released (state 3).
 *
 * Turns off laser and returns to wait state once the button is released.
 *
 * @param shoot_task Pointer to ShootTask structure.
 */
void shoot_task_state_3_shoot(ShootTask *shoot_task)
{
    HAL_GPIO_WritePin(GPIOB, shoot_task->laser_gpio, GPIO_PIN_RESET);
    if (shoot_task->button == 0) {
        shoot_task->state = 1;
    }
}
