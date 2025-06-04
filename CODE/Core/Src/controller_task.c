/*
 * controller_task.c
 *
 *  Created on: Jun 3, 2025
 *      Author: kaidc
 */

#include "motor_driver.h"
#include "controller_task.h"
#include "stm32f4xx_hal_tim.h"
#include <stdio.h>

// A function to run the appropriate state of the task
void controller_task_run(ControllerTask *controller_task)
{    // Check for a valid state
    if (controller_task->state >= 0 && controller_task->state < controller_task->num_states)
    {
        // Index the list of state functions and then call the appropriate
        // method while passing in (this) task_1 object

    	// game_task is a pointer right now
    	// -> dereferences pointer and accesses class member in one step
    	// the same as (*ptr).member

        controller_task->state_list[controller_task->state](controller_task);

    }
    // Big problems if the state is invalid
    else
    {

        while(1){}
    }

}

// A function to initialize the task
void controller_task_state_0_init(ControllerTask *controller_task)
{
	// initialize prev_error and integral stuct variables to 0
	controller_task->prev_error = 0;

	//set current encoder value to zero
	HAL_TIM_Encoder_Start(controller_task->htim_encoder, TIM_CHANNEL_ALL);

	// Enable motor
	enable(controller_task->motor);

	HAL_ADC_Start(controller_task->hadc);
	HAL_ADC_PollForConversion(controller_task->hadc, 10);
	uint32_t adc_val = HAL_ADC_GetValue(controller_task->hadc);
	HAL_ADC_Stop(controller_task->hadc);
	controller_task->pot_zero = adc_val;

}

//stays in state 1
void controller_task_state_1_calc_vel(ControllerTask *controller_task)
{
    // Constants for PID and sampling time
    const float delta_time = 0.02f;  // Adjust to control loop frequency from encoder sampling(seconds)
    const float Kp = 1.0f;
    const float Kd = 0.0f;

    // --- Read desired velocity from ADC (potentiometer) ---
    HAL_ADC_Start(controller_task->hadc);
    HAL_ADC_PollForConversion(controller_task->hadc, 10);
    uint32_t adc_val = HAL_ADC_GetValue(controller_task->hadc);
    HAL_ADC_Stop(controller_task->hadc);

    adc_val -= controller_task->pot_zero;

    const float MAX_ADC = 4095.0f;       // 12-bit ADC max value
    const float MAX_VELOCITY = 1.0f;  // Adjust based on system units

    float desired_velocity = ((float)adc_val / MAX_ADC) * MAX_VELOCITY;

    // --- Calculate actual velocity from encoder ticks --- ** might need to account for overload on timer
    static int32_t last_ticks = 0;
    int32_t current_ticks = __HAL_TIM_GET_COUNTER(controller_task->htim_encoder);
    int32_t delta_ticks = current_ticks - last_ticks;
    last_ticks = current_ticks;

    float current_velocity = (float)delta_ticks / delta_time; // Scale to velocity units

    // --- PID control calculation ---
    float error = desired_velocity - current_velocity;
    float derivative = (error - controller_task->prev_error) / delta_time;
    controller_task->prev_error = error;

    float control_signal = Kp * error + Kd * derivative;

    // Saturate control signal to acceptable motor input range, e.g., PWM duty cycle [-100, 100]
    if(control_signal > 100.0f) control_signal = 100.0f;
    else if(control_signal < -100.0f) control_signal = -100.0f;

    // Apply control signal to motor
    set_duty(controller_task->motor, (int32_t)control_signal);
}


