/**
*  @file game_task.h
 *  @brief Header file for Zap'em Blast'em Robots game task logic.
 *  @author Andrew Carr and Kai De La Cruz
 *
 *  Created on: May 22, 2025

 */

#ifndef INC_GAME_TASK_H_
#define INC_GAME_TASK_H_

#include <stdint.h> // Allows use of standard integer types
#include "stm32f4xx_hal.h"  // or your STM32 family

/**
 * @brief Forward declaration of SoundTask structure.
 */
typedef struct SoundTask SoundTask;

/**
 * @brief Forward declaration of PhotoresistorTask structure.
 */
typedef struct PhotoresistorTask PhotoresistorTask;

/**
 * @brief Forward declaration of GameTask structure.
 */
typedef struct GameTask GameTask;

/**
 * @brief Function pointer type for GameTask state functions.
 */
typedef void (*game_fcn_t)(GameTask *game_task);

/**
 * @brief Structure for game task configuration and state tracking.
 *
 * Holds scoring logic, timing values, peripherals, and function pointers
 * used in the game state machine.
 */
struct GameTask
{
<<<<<<< HEAD
    int32_t             state;          /**< Current game state index */
    int32_t             num_states;     /**< Total number of game states */
    int32_t             play_flag;      /**< Flag indicating game is active */
    int32_t             score_red;      /**< Score for red player */
    int32_t             score_blue;     /**< Score for blue player */
    int32_t             score_red_prev; /**< Last red score shown on LCD */
    int32_t             score_blue_prev;/**< Last blue score shown on LCD */
    int32_t             score_thresh;   /**< Score needed to win */
    int32_t             num;            /**< Counter for LCD init check */
    uint32_t            delay_start;    /**< Start time for delay */
    int32_t             delay_flag;     /**< Delay active flag */
    uint32_t            delay;          /**< Delay duration in ms */
    uint32_t            end_delay;      /**< Delay after game ends */

    TIM_HandleTypeDef   *htim;          /**< Timer for delay tracking */
    SoundTask           *sound_task_ptr;/**< Pointer to sound task */
    PhotoresistorTask   *red_photoresistor_task_ptr;
    /**< Pointer to red sensor task */
    PhotoresistorTask   *blue_photoresistor_task_ptr;
    /**< Pointer to blue sensor task */
    motor_t             *mred;          /**< Pointer to red motor */
    motor_t             *mblue;         /**< Pointer to blue motor */
    I2C_HandleTypeDef   *i2c_handle;    /**< I2C handle for LCD */
    game_fcn_t          state_list[];   /**< Function pointer array */
=======
    int32_t     		state;
    int32_t     		num_states;
    int32_t				play_flag;
    int32_t				score_red;
    int32_t				score_blue;
    int32_t 			score_red_prev;
    int32_t 			score_blue_prev;
    int32_t				score_thresh;
    int32_t				num;
    uint32_t			delay_start;
    int32_t				delay_flag;
    uint32_t			delay;
    uint32_t			end_delay;
    TIM_HandleTypeDef 	*htim;
    SoundTask			*sound_task_ptr;
    PhotoresistorTask 	*red_photoresistor_task_ptr;
    PhotoresistorTask 	*blue_photoresistor_task_ptr;
    I2C_HandleTypeDef 	*i2c_handle;
    game_fcn_t 			state_list[];
>>>>>>> parent of 1454172 (Merge branch 'main' of https://github.com/andrewpatcarr/zap_em__blast_em)
};

/**
 * @brief Initializes the GameTask (state 0).
 *
 * @param game_task Pointer to the GameTask structure.
 */
void game_task_state_0_init(GameTask *game_task);

/**
 * @brief Executes the current GameTask state.
 *
 * @param game_task Pointer to the GameTask structure.
 */
void game_task_run(GameTask *game_task);

/**
 * @brief Handles home screen and game start prompt (state 1).
 *
 * @param game_task Pointer to the GameTask structure.
 */
void game_task_state_1_home(GameTask *game_task);

/**
 * @brief Handles main gameplay and scoring logic (state 2).
 *
 * @param game_task Pointer to the GameTask structure.
 */
void game_task_state_2_play(GameTask *game_task);

/**
 * @brief Ends the game and resets variables (state 3).
 *
 * @param game_task Pointer to the GameTask structure.
 */
void game_task_state_3_end(GameTask *game_task);

#endif /* INC_GAME_TASK_H_ */
