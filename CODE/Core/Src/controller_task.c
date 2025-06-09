/*
 * controller_task.c
 *
 *  Created on: Jun 3, 2025
 *      Author: kaidc
 */

#include "motor_driver.h"
#include "controller_task.h"
#include "stm32f4xx_hal_tim.h"
#include "encoder_driver.h"
#include <stdio.h>

// A function to run the appropriate state of the task
typedef void (*StateFunc)(ControllerTask *);
void controller_task_run(ControllerTask *controller_task)
{
    if (controller_task->state >= 0 && controller_task->state < controller_task->num_states)
    {
        controller_task->state_list[controller_task->state](controller_task);
    }
    else
    {
        while (1) {}
    }
}

// init stuff and zero pot
void controller_task_state_0_init(ControllerTask *controller_task)
{
    controller_task->prev_error = 0;
    enable(controller_task->motor);
    // record ADC zero (center/home)
    controller_task->pot_zero = controller_task->adc_val;

    controller_task->state = 1;
}

void controller_task_state_1_pos(ControllerTask *controller_task){
	int32_t des_pos = controller_task->dir * (controller_task->adc_val - controller_task->pot_zero);
	int32_t error = des_pos - controller_task->encoder->ticks;
	if (error > 100) error = 99;
	if (error < -100) error = -99;

	read_encoder(controller_task->encoder);	// updates p_enc->ticks
//	int32_t current_ticks = __HAL_TIM_GET_COUNTER(controller_task->htim_encoder);
//	int32_t delta_ticks   = current_ticks - controller_task->prev_ticks;
//	controller_task->prev_ticks = current_ticks;

	controller_task->current_time = __HAL_TIM_GET_COUNTER(controller_task->htim_dt);
	uint32_t dt_ticks = (controller_task->current_time >= controller_task->prev_time) ?
						(controller_task->current_time - controller_task->prev_time) :
						(0xFFFFFFFF - controller_task->prev_time + controller_task->current_time);
	if (dt_ticks == 0) return;
	int32_t delta_time      = dt_ticks / 1000.0f;
	controller_task->integral_error += error * delta_time;
	int32_t control = controller_task->k_p * error + controller_task->k_i * controller_task->integral_error;

	set_duty(controller_task->motor,control);

}
