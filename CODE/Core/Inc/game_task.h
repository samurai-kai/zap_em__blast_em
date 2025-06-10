/**
 *  @file game_task.h
 *  @brief Header file for the ADC task system
 *  @author Andrew Carr and Kai De La Cruz
 *
 *  Created on: May 22, 2025
 *
 */

#ifndef INC_GAME_TASK_H_
#define INC_GAME_TASK_H_

#include <stdint.h>
#include "stm32f4xx_hal.h"

/**
 * @brief Forward declaration for SoundTask.
 */
typedef struct SoundTask SoundTask;
/**
 * @brief Forward declaration for PhotoresistorTask.
 */
typedef struct PhotoresistorTask PhotoresistorTask;
/**
 * @brief Forward declaration for GameTask.
 */
typedef struct GameTask GameTask;

/**
 * @brief Function pointer type for ADC state functions.
 *
 * Each state function must take a pointer to ADCTask as its argument.
 */
typedef void (*game_fcn_t)(GameTask *game_task);

/**
 * @brief Structure that represents the game task state and configuration.
 *
 * This structure holds all the variables and function pointers
 * necessary to run game operations within the task-based state machine.
 */
struct GameTask
{
	int32_t             state;            				/**< Current state index */
	int32_t             num_states;       				/**< Total number of states */
    int32_t				play_flag;						/**< game in play mode */
    int32_t				score_red;						/**< Red fighter's score */
    int32_t				score_blue;						/**< Blue fighter's score */
    int32_t 			score_red_prev;					/**< Red's previous score */
    int32_t 			score_blue_prev;				/**< Blue's previous score */
    int32_t				score_thresh;					/**< Score needed to to win*/
    int32_t				num;							/**< Counter variable */
    uint32_t			red_delay_start;				/**< Red delay start time */
    uint32_t			blue_delay_start;				/**< Blue delay start time */
    int32_t				red_delay_flag;					/**< Red in delay or not */
    int32_t				blue_delay_flag;				/**< Blue in delay or not */
    uint32_t			delay;							/**< Length of delay in ms */
    uint32_t			end_delay;						/**< Length of end of game delay */
    SoundTask			*sound_task_ptr;				/**< Pointer to SoundTask object */
    PhotoresistorTask 	*red_photoresistor_task_ptr; 	/**< Pointer to Red PhotoresistorTask object */
    PhotoresistorTask 	*blue_photoresistor_task_ptr; 	/**< Pointer to Blue PhotoresistorTask object */
    motor_t 			*mred;							/**< Pointer to red motor object */
    motor_t 			*mblue;							/**< Pointer to red motor object */
    I2C_HandleTypeDef 	*i2c_handle;					/**< Pointer I2C handle */
    game_fcn_t 			state_list[];					/**< Function pointer list for states */
};

/**
 * @brief Initializes the game task (state 0).
 *
 * Clears and sets up the LCD, then advances to state 1.
 *
 * @param game_task Pointer to the GameTask instance.
 */
void game_task_state_0_init(GameTask *game_task);
/**
 * @brief Executes the current game task state.
 *
 * @param game_task Pointer to the GameTask instance.
 */
void game_task_run(GameTask *game_task);
/**
 * @brief Waits for player to initiate the game (state 1).
 *
 * Displays title and instructions. When play_flag is set, transitions
 * to the next state and triggers the start sound.
 *
 * @param game_task Pointer to the GameTask instance.
 */
void game_task_state_1_home(GameTask *game_task);
/**
 * @brief Handles gameplay logic and scoring (state 2).
 *
 * Increments scores based on photoresistor hit flags, updates LCD with
 * score, and transitions to end state when a score threshold is reached.
 *
 * @param game_task Pointer to the GameTask instance.
 */
void game_task_state_2_play(GameTask *game_task);
/**
 * @brief Displays game over screen and resets (state 3).
 *
 * Shows a winning message based on who reached the score threshold
 * first. After a delay, resets the game state and variables.
 *
 * @param game_task Pointer to the GameTask instance.
 */
void game_task_state_3_end(GameTask *game_task);

#endif /* INC_GAME_TASK_H_ */
