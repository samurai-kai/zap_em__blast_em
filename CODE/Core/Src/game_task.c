/*
 * game_task.c
 *
 *  Created on: May 22, 2025
 *      Author: andrewcarr
 */

#include "game_task.h"

// A function to run the appropriate state of the task
void game_task_run(void)
{    // Check for a valid state
    if (game_task.state >= 0 && game_task.state < game_task.num_states)
    {
        // Index the list of state functions and then call the appropriate
        // method while passing in (this) task_1 object
        game_task.state_list[game_task.state]();

    }
    // Big problems if the state is invalid
    else
    {

        while(1){}
    }

}


// A function to initialize the task
void game_task_state_0_init(void)
{
    //add init stuff, display

}
// A function to implement state 1 of the task
// keeps for user start sequence to happen then starts the game
// Prints message and sets sound flag when game starts
void game_task_state_1_home(void)
{
    //play_flg enabled from button task within shoot task?? or make button task

	if (game_task.play_flg == 1){
    	game_task.state = 2;
    }

}
// A function to implement state 2 which is for when the game is being played
// This keeps track of score, prints score messages
void game_task_state_2_play(void)
{
	// add thing that prints score of each on the LCD


	//when the game is over
	if (game_task.score_red > game_task.score_thresh || game_task.score_blue > game_task.score_thresh){
		// print win message and set end sound
		game_task.state = 3;
	}

}
// A function to implement state 3
// Ends the game when the score threshold has been met
// Prints messages and sets sound flags
void game_task_state_3_end(void)
{
	// lowkey might not be needed
	// does need to reset everything but could do in above
	game_task.state = 1;
}

