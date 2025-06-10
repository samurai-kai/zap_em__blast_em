/**
 *  @file sound_task.c
 *  @brief Sound Task state machine for playing audio files corresponding to game
 *  mechanics
 *	@author Andrew Carr
 *
 *
 *  Created on: Jun 5, 2025
 *
 */

#include "laser_buf.h"
#include "game_over_buf.h"
#include "start_buf.h"
#include "hit_buf.h"
#include "sound_task.h"
#include <stdio.h>

volatile uint32_t sample_index   = 0;
volatile uint8_t  pwm_div_count  = 0;
volatile uint32_t audio_buf_len;
volatile uint32_t playing = 0;
const int16_t *audio_buf_ptr;

/**
 * @brief Runs the current state of the SoundTask.
 *
 * Checks whether the current state index is valid. If so, it calls the
 * corresponding state function from the task's function pointer array.
 *
 * @param sound_task Pointer to the SoundTask instance.
 */

void sound_task_run(SoundTask *sound_task)
{
    if (sound_task->state >= 0 && sound_task->state < sound_task->num_states)
    {
    	sound_task->state_list[sound_task->state](sound_task);

    }
    else
    {

        while(1){}
    }
}


/**
 * @brief Initializes the sound task (state 0).
 *
 * Goes to state 1, waiting for sound flags.
 *
 * @param sound_task Pointer to the SoundTask instance.
 */

void sound_task_state_0_init(SoundTask *sound_task)
{
	sound_task->state = 1;

}
/**
 * @brief Waits for sound flags (state 1).
 *
 * Waits for flags to go high and then sends the state machine into the
 * corresponding state.
 *
 * @param sound_task Pointer to the SoundTask instance.
 */

void sound_task_state_1_wait(SoundTask *sound_task)
{

	if(sound_task->hit_snd == 1){
		sound_task->state = 3;
	}
	else if(sound_task->win_snd == 1){
		sound_task->state = 4;
	}
	else if(sound_task->start_snd == 1){
		sound_task->state = 5;
	}
	else if (sound_task->laser_snd == 1){
		sound_task->state = 2;
	}

}
/**
 * @brief Starts the interrupts and sets the array pointer for the laser sound(state 2).
 *
 * Assigns the corresponding audio buffer and length to the extern variables.
 * Then, restarts interrupts and PWM for the speaker.
 *
 * @param sound_task Pointer to the SoundTask instance.
 */

void sound_task_state_2_laser(SoundTask *sound_task)
{
	if (playing) return;
	if (sound_task->laser_snd == 1){
		audio_buf_ptr = laser_buf;
		audio_buf_len = laser_buf_len;
		playing = 1;
		sample_index = 0;
		__HAL_TIM_SET_COMPARE(sound_task->htim, sound_task->chan, 0);
		__HAL_TIM_CLEAR_FLAG(sound_task->htim, TIM_FLAG_UPDATE);
		__HAL_TIM_ENABLE_IT(sound_task->htim, TIM_IT_UPDATE);
		HAL_TIM_PWM_Start(sound_task->htim, sound_task->chan);
		sound_task->laser_snd = 0;
	}

	if (playing == 0){
		sound_task->state = 6;
	}

}
/**
 * @brief Starts the interrupts and sets the array pointer for the hit sound(state 3).
 *
 * Assigns the corresponding audio buffer and length to the extern variables.
 * Then, restarts interrupts and PWM for the speaker.
 *
 * @param sound_task Pointer to the SoundTask instance.
 */

void sound_task_state_3_hit(SoundTask *sound_task)
{
	if (playing) {
		sound_task->laser_snd = 0;
		return;
	}
	if (sound_task->hit_snd == 1){
		audio_buf_ptr = hit_buf;
		audio_buf_len = hit_buf_len;
		playing = 1;
		sample_index = 0;
		__HAL_TIM_SET_COMPARE(sound_task->htim, sound_task->chan, 0);
		__HAL_TIM_CLEAR_FLAG(sound_task->htim, TIM_FLAG_UPDATE);
		__HAL_TIM_ENABLE_IT(sound_task->htim, TIM_IT_UPDATE);
		HAL_TIM_PWM_Start(sound_task->htim, sound_task->chan);
		sound_task->hit_snd = 0;
	}

	if (playing == 0){
		sound_task->state = 6;
	}

}
/**
 * @brief Starts the interrupts and sets the array pointer for the win sound(state 4).
 *
 * Assigns the corresponding audio buffer and length to the extern variables.
 * Then, restarts interrupts and PWM for the speaker.
 *
 * @param sound_task Pointer to the SoundTask instance.
 */

void sound_task_state_4_win(SoundTask *sound_task)
{
	if (playing){
		sound_task->laser_snd = 0;
		sound_task->hit_snd = 0;
		return;
	}
	if (sound_task->win_snd == 1){
		audio_buf_ptr = game_over_buf;
		audio_buf_len = game_over_buf_len;
		playing = 1;
		sample_index = 0;
		__HAL_TIM_SET_COMPARE(sound_task->htim, sound_task->chan, 0);
		__HAL_TIM_CLEAR_FLAG(sound_task->htim, TIM_FLAG_UPDATE);
		__HAL_TIM_ENABLE_IT(sound_task->htim, TIM_IT_UPDATE);
		HAL_TIM_PWM_Start(sound_task->htim, sound_task->chan);
		sound_task->win_snd = 0;
	}

	if (playing == 0){
		sound_task->state = 6;
	}
}
/**
 * @brief Starts the interrupts and sets the array pointer for the start sound(state 5).
 *
 * Assigns the corresponding audio buffer and length to the extern variables.
 * Then, restarts interrupts and PWM for the speaker.
 *
 * @param sound_task Pointer to the SoundTask instance.
 */

void sound_task_state_5_start(SoundTask *sound_task)
{
	if (playing) {
		sound_task->laser_snd = 0;
		return;
	}
	if (sound_task->start_snd == 1){
		audio_buf_ptr = start_buf;
		audio_buf_len = start_buf_len;
		playing = 1;
		sample_index = 0;
		__HAL_TIM_SET_COMPARE(sound_task->htim, sound_task->chan, 0);
		__HAL_TIM_CLEAR_FLAG(sound_task->htim, TIM_FLAG_UPDATE);
		__HAL_TIM_ENABLE_IT(sound_task->htim, TIM_IT_UPDATE);
		HAL_TIM_PWM_Start(sound_task->htim, sound_task->chan);
		sound_task->start_snd = 0;
	}

	if (playing == 0){
		sound_task->state = 6;
	}

}
/**
 * @brief Stops the interrupts and PWM for speaker after sound played (state 6)
 *
 * Stops interrupts and PWM for speaker then goes back to the waiting state.
 *
 * @param sound_task Pointer to the SoundTask instance.
 */

void sound_task_state_6_stop(SoundTask *sound_task){
	__HAL_TIM_DISABLE_IT(sound_task->htim, TIM_IT_UPDATE);
	HAL_TIM_PWM_Stop(sound_task->htim, sound_task->chan);
	sound_task->state = 1;
}
