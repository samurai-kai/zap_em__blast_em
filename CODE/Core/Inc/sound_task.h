/**
 *  @file sound_task.c
 *  @brief  Header file for the sound task system
 *	@author Andrew Carr
 *
 *
 *  Created on: May 22, 2025
 *
 */


#ifndef INC_SOUND_TASK_H_
#define INC_SOUND_TASK_H_

#include <stdint.h> // Allows use of standard integer types
#include "stm32f4xx_hal.h"

/**
 * @brief Forward declaration for SoundTask.
 */
typedef struct SoundTask SoundTask;
/**
 * @brief Function pointer type for sound state functions.
 *
 * Each state function must take a pointer to SoundTask as its argument.
 */
typedef void (*sound_fcn_t)(SoundTask *sound_task);

#define SAMPLE_RATE   44100U					/**< Audio sample rate */
#define PWM_FREQ      88200U					/**< Output Timer PWM frequency */
#define STEP_RATIO    (PWM_FREQ / SAMPLE_RATE)  /**< Step ratio for  */

extern const int16_t *audio_buf_ptr;   			/**< Pointer to current sound buffer */
extern volatile uint32_t audio_buf_len; 		/**< Length current sound buffer */

extern volatile uint32_t sample_index;			/**< Index of step in sound buffer */
extern volatile uint8_t  pwm_div_count;			/**< counter for stepping through audio buf */
extern volatile uint32_t playing;				/**< Flag for when a sound is playing */


/**
 * @brief Structure that represents the sound task state and configuration.
 *
 * This structure holds all the contextual information and function pointers
 * necessary to play sounds in the task-based state machine.
 */
struct SoundTask
{
	int32_t     		state;					/**< Current state index */
    int32_t             num_states;       		/**< Total number of states */
    int32_t				laser_snd;				/**< Flag for playing laser sound */
    int32_t				hit_snd;				/**< Flag for playing hit sound */
    int32_t				start_snd;				/**< Flag for playing start sound */
    int32_t				win_snd;				/**< Flag for playing win sound */
    uint32_t			chan;					/**< PWM channel for sound timer */
    TIM_HandleTypeDef 	*htim;					/**< Timer for sound */

    sound_fcn_t 		state_list[];			/**< Function pointer list for states */
};

/**
 * @brief Executes the current sound task state.
 *
 * @param sound_task Pointer to the SoundTask instance.
 */
void sound_task_run(SoundTask *sound_task);
/**
 * @brief Initializes the sound task (state 0).
 *
 * Goes to state 1, waiting for sound flags.
 *
 * @param sound_task Pointer to the SoundTask instance.
 */

void sound_task_state_0_init(SoundTask *sound_task);
/**
 * @brief Waits for sound flags (state 1).
 *
 * Waits for flags to go high and then sends the state machine into the
 * corresponding state.
 *
 * @param sound_task Pointer to the SoundTask instance.
 */

void sound_task_state_1_wait(SoundTask *sound_task);
/**
 * @brief Starts the interrupts and sets the array pointer for the laser sound(state 2).
 *
 * Assigns the corresponding audio buffer and length to the extern variables.
 * Then, restarts interrupts and PWM for the speaker.
 *
 * @param sound_task Pointer to the SoundTask instance.
 */
void sound_task_state_2_laser(SoundTask *sound_task);
/**
 * @brief Starts the interrupts and sets the array pointer for the hit sound(state 3).
 *
 * Assigns the corresponding audio buffer and length to the extern variables.
 * Then, restarts interrupts and PWM for the speaker.
 *
 * @param sound_task Pointer to the SoundTask instance.
 */
void sound_task_state_3_hit(SoundTask *sound_task);
/**
 * @brief Starts the interrupts and sets the array pointer for the win sound(state 4).
 *
 * Assigns the corresponding audio buffer and length to the extern variables.
 * Then, restarts interrupts and PWM for the speaker.
 *
 * @param sound_task Pointer to the SoundTask instance.
 */
void sound_task_state_4_win(SoundTask *sound_task);
/**
 * @brief Starts the interrupts and sets the array pointer for the start sound(state 5).
 *
 * Assigns the corresponding audio buffer and length to the extern variables.
 * Then, restarts interrupts and PWM for the speaker.
 *
 * @param sound_task Pointer to the SoundTask instance.
 */
void sound_task_state_5_start(SoundTask *sound_task);
/**
 * @brief Stops the interrupts and PWM for speaker after sound played (state 6)
 *
 * Stops interrupts and PWM for speaker then goes back to the waiting state.
 *
 * @param sound_task Pointer to the SoundTask instance.
 */

void sound_task_state_6_stop(SoundTask *sound_task);


#endif /* INC_SOUND_TASK_H_ */
