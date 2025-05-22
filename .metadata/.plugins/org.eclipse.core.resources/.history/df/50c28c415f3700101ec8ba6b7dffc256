/*
 * game_task.h
 *
 *  Created on: May 22, 2025
 *      Author: andrewcarr
 */

#ifndef INC_GAME_TASK_H_
#define INC_GAME_TASK_H_

// function must have no input arguments and have no return
typedef void (*state_fcn_t)(void);

// A datatype for a structure to hold task configuration and state.
// Additional fields can be added as desired.
typedef struct game_task_cfg
{
    int32_t     state;
    int32_t     num_states;
    int32_t		play_flg;
    int32_t		score_red;
    int32_t		score_blue;
    state_fcn_t state_list[];
} game_task_cfg_t;

// A prototype for each function implemented in task_1.c
void game_task_state_0_init(void);
void game_task_run(void);
void game_task_state_1_home(void);
void game_task_state_2_play(void);
void game_task_state_3_end(void);

#endif /* INC_GAME_TASK_H_ */
