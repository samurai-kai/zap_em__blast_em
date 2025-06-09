/**
 *  @file motor_driver.c
 *  @brief Motor control functions for setting duty and direction via PWM.
 *  @author Kai De La Cruz
 *
 *  Created on: May 29, 2025
 */

#include "motor_driver.h"
#include "encoder_driver.h"
#include <stdint.h>

<<<<<<< HEAD
/// @brief Maximum PWM duty cycle value.
=======

>>>>>>> parent of 1454172 (Merge branch 'main' of https://github.com/andrewpatcarr/zap_em__blast_em)
#define PWM_MAX 4800

/**
 * @brief Sets motor duty cycle and direction.
 *
 * Positive values drive forward; negative values drive reverse.
 * Duty is scaled from percentage to raw PWM range.
 *
 * @param p_mot Pointer to the motor structure
 * @param duty Desired duty cycle percentage (-100 to 100)
 */
void set_duty(motor_t* p_mot, int32_t duty)
{
    duty = duty*PWM_MAX/100;
	if(duty>=0)
	{
	__HAL_TIM_SET_COMPARE(p_mot->tim, p_mot->chan1, duty);
	__HAL_TIM_SET_COMPARE(p_mot->tim, p_mot->chan2, 0);
    }
	else if(duty<0)
	{
	__HAL_TIM_SET_COMPARE(p_mot->tim, p_mot->chan1, 0);
	__HAL_TIM_SET_COMPARE(p_mot->tim, p_mot->chan2, abs(duty));
	}

}

/**
 * @brief Disables motor output by setting both channels to 0.
 *
 * @param p_mot Pointer to the motor structure
 */
void disable(motor_t* p_mot)
{
	__HAL_TIM_SET_COMPARE(p_mot->tim, p_mot->chan1, 0);
	__HAL_TIM_SET_COMPARE(p_mot->tim, p_mot->chan2, 0);

}

/**
 * @brief Enables motor by setting both channels to maximum PWM.
 *
 * @param p_mot Pointer to the motor structure
 */
void enable(motor_t* p_mot)
{
	__HAL_TIM_SET_COMPARE(p_mot->tim, p_mot->chan1, PWM_MAX);
	__HAL_TIM_SET_COMPARE(p_mot->tim, p_mot->chan2, PWM_MAX);
}

<<<<<<< HEAD
/**
 * @brief Drives motor toward a target position using P-control.
 *
 * Error is limited to +/-99 to avoid large spikes. Applies proportional
 * control using gain `k_p`.
 *
 * @param p_mot Pointer to the motor structure
 * @param k_p   Proportional gain constant
 * @param where Target position
 * @param from  Current position
 */
void go_to(motor_t *p_mot, int k_p, int where, int from)
{
    int error = where - from;
    if (error > 100) error = 99;
    if (error < -100) error = -99;
    set_duty(p_mot, k_p * error);
=======
void go_to(motor_t *p_mot, int k_p, int where, int from){

	int error = where-from;

	if (error >100){
		error = 99;
	}
	if (error < -100){
		error = -99;
	}

	set_duty(p_mot, k_p*error);



>>>>>>> parent of 1454172 (Merge branch 'main' of https://github.com/andrewpatcarr/zap_em__blast_em)
}
