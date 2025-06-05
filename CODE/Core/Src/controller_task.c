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

void read_adc_channels_scan_mode(ADC_HandleTypeDef *hadc, int32_t *adc_val_6, int32_t *adc_val_7)
{
    HAL_ADC_Start(hadc);

    // Wait for first conversion (ADC6)
    if (HAL_ADC_PollForConversion(hadc, 10) == HAL_OK) {
        *adc_val_6 = HAL_ADC_GetValue(hadc);
    }

    // Wait for second conversion (ADC7)
    if (HAL_ADC_PollForConversion(hadc, 10) == HAL_OK) {
        *adc_val_7 = HAL_ADC_GetValue(hadc);
    }

    HAL_ADC_Stop(hadc);
}

void controller_task_state_0_init(ControllerTask *controller_task)
{
    controller_task->prev_error = 0;
    HAL_TIM_Encoder_Start(controller_task->htim_encoder, TIM_CHANNEL_ALL);
    enable(controller_task->motor);

    int32_t adc_val_6 = 0, adc_val_7 = 0;
    HAL_Delay(1);
    read_adc_channels_scan_mode(controller_task->hadc, &adc_val_6, &adc_val_7);

    controller_task->pot_zero = (controller_task->color == 0) ? adc_val_7 : adc_val_6;

    controller_task->state = 1;
}

void controller_task_state_1_calc_vel(ControllerTask *controller_task)
{
    const float Kp = 20.0f;
    const float Kd = 0.0f;
    uint32_t t0 = __HAL_TIM_GET_COUNTER(controller_task->htim_dt);

    int32_t adc_val_6 = 0, adc_val_7 = 0;
    HAL_Delay(1);
    read_adc_channels_scan_mode(controller_task->hadc, &adc_val_6, &adc_val_7);

    int32_t adc_val = (controller_task->color == 0) ? adc_val_7 : adc_val_6;
    adc_val -= controller_task->pot_zero;

    const float MAX_ADC = 4095.0f;
    const float MAX_VELOCITY = 1.0f;
    float desired_velocity = ((float)adc_val / MAX_ADC) * MAX_VELOCITY;

    static volatile int32_t last_ticks = 0;
    int32_t current_ticks = __HAL_TIM_GET_COUNTER(controller_task->htim_encoder);
    int32_t delta_ticks = current_ticks - last_ticks;
    last_ticks = current_ticks;

    uint32_t t1 = __HAL_TIM_GET_COUNTER(controller_task->htim_dt);
    uint32_t dt_ticks = (t1 >= t0) ? (t1 - t0) : (0xFFFFFFFF - t0 + t1);
    float delta_time = dt_ticks / 875000.0f;;

    float current_velocity = (float)delta_ticks / delta_time;
    float error = desired_velocity - current_velocity;
    float derivative = (error - controller_task->prev_error) / delta_time;
    controller_task->prev_error = error;

    controller_task->control_signal = Kp * error + Kd * derivative;
    float control_signal = controller_task->control_signal;
    if (control_signal > 100.0f) control_signal = 100.0f;
    else if (control_signal < -100.0f) control_signal = -100.0f;

    set_duty(controller_task->motor, (int32_t)control_signal);
}
