/**
 *  @file motor_driver.c
 *  @brief Motor control functions for PWM-based direction and speed handling.
 *  @author Kai De La Cruz
 *
 *  Created on: May 29, 2025
 */

#include "motor_driver.h"
#include "encoder_driver.h"
#include "stm32f4xx_hal.h"
#include <stdint.h>

/// @brief Maximum PWM value used for scaling duty cycle
#define PWM_MAX         4800

/**
 * @brief Sets the motor duty cycle and direction using PWM.
 *
 * @param p_mot Pointer to the motor structure
 * @param duty  Desired duty cycle (-100 to 100 percent)
 */
void set_duty(motor_t* p_mot, int32_t duty)
{
    duty = duty * PWM_MAX / 100;
    if (duty >= 0) {
        __HAL_TIM_SET_COMPARE(p_mot->tim, p_mot->chan1, duty);
        __HAL_TIM_SET_COMPARE(p_mot->tim, p_mot->chan2, 0);
    } else {
        __HAL_TIM_SET_COMPARE(p_mot->tim, p_mot->chan1, 0);
        __HAL_TIM_SET_COMPARE(p_mot->tim, p_mot->chan2, abs(duty));
    }
}

/**
 * @brief Disables the motor by setting both channels to 0% duty.
 *
 * @param p_mot Pointer to the motor structure
 */
void disable(motor_t* p_mot)
{
    __HAL_TIM_SET_COMPARE(p_mot->tim, p_mot->chan1, 0);
    __HAL_TIM_SET_COMPARE(p_mot->tim, p_mot->chan2, 0);
}

/**
 * @brief Enables the motor by setting both channels to 100% duty.
 *
 * @param p_mot Pointer to the motor structure
 */
void enable(motor_t* p_mot)
{
    __HAL_TIM_SET_COMPARE(p_mot->tim, p_mot->chan1, PWM_MAX);
    __HAL_TIM_SET_COMPARE(p_mot->tim, p_mot->chan2, PWM_MAX);
}

/**
 * @brief Moves the motor toward a target using proportional control.
 *
 * @param p_mot Pointer to the motor structure
 * @param k_p   Proportional gain
 * @param where Target position
 * @param from  Current position
 */
void go_to(motor_t *p_mot, int k_p, int where, int from)
{
    int error = where - from;
    if (error > 100) error = 99;
    if (error < -100) error = -99;
    set_duty(p_mot, k_p * error);
}
