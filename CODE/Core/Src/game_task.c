/*
 * game_task.c
 *
 *  Created on: May 22, 2025
 *      Author: andrewcarr
 */

#include "game_task.h"
#include "sound_task.h"
#include "photoresistor_task.h"
#include "lcd.h"

#include <stdio.h>

// A function to run the appropriate state of the task
void game_task_run(GameTask *game_task)
{    // Check for a valid state
    if (game_task->state >= 0 && game_task->state < game_task->num_states)
    {
        // Index the list of state functions and then call the appropriate
        // method while passing in (this) task_1 object

    	// game_task is a pointer right now
    	// -> dereferences pointer and accesses class member in one step
    	// the same as (*ptr).member

        game_task->state_list[game_task->state](game_task);

    }
    // Big problems if the state is invalid
    else
    {

        while(1){}
    }

}


// A function to initialize the task
void game_task_state_0_init(GameTask *game_task)
{
    //add init stuff, display
	lcd_init(game_task->i2c_handle);
	game_task->state = 1;

}
// A function to implement state 1 of the task
// keeps for user start sequence to happen then starts the game
// Prints message and sets sound flag when game starts
void game_task_state_1_home(GameTask *game_task)
{
    //play_flg enabled from button task within shoot task?? or make button task
	lcd_write(0, 0, "Hello World"); // x,y,message, display is 20 wide 4 tall
	lcd_write(0,1,"Game test, game test");// 20 long message
	if (game_task->play_flag == 1){
    	game_task->state = 2;
    	game_task->sound_task_ptr->start_snd = 1; // sets start sound flag for sound task to play it
    }

}
// A function to implement state 2 which is for when the game is being played
// This keeps track of score, prints score messages
void game_task_state_2_play(GameTask *game_task)
{
	char r_score[5];
	char b_score[5];


	if (game_task->red_photoresistor_task_ptr->hit_flag){
		game_task->score_red++;
		game_task->red_photoresistor_task_ptr->hit_flag = 0;
		//might need to add delay so that won't get extra hits for extended period of hitting
	}
	if (game_task->blue_photoresistor_task_ptr->hit_flag){
		game_task->score_blue++;
		game_task->blue_photoresistor_task_ptr->hit_flag = 0;
		//might need to add delay so that won't get extra hits for extended period of hitting
	}

	// add thing that prints score of each on the LCD
	//maybe only do once then adjust the score through a direct print index
	if (game_task->num == 0){
//		lcd_write(0,0,"Zap'em Blast'em     ");
//		lcd_write(0,1,"     First to 5     ");
//		lcd_write(0,2,"Red:  0  Zaps       ");
//		lcd_write(0,3,"Blue: 0  Blasts     ");
		//             01234567890123456789
		game_task->num++;
	}

	// check to see if score changed for the lcd
	if (game_task->score_red != game_task->score_red_prev){
//
//		sprintf(r_score,"%ld",game_task->score_red);
//		lcd_write(6,2,r_score);
//		game_task->score_red_prev = game_task->score_red;
	}
	if (game_task->score_blue != game_task->score_blue_prev){
//
//		sprintf(b_score,"%ld",game_task->score_blue);
//		lcd_write(6,3,b_score);
//		game_task->score_blue_prev = game_task->score_blue;
	}


	// check if someone won
//	if (game_task->score_red > game_task->score_thresh || game_task->score_blue > game_task->score_thresh){
//		// print win message and set end sound
//		lcd_write(0,0,"Zap'em Blast'em     ");
//		lcd_write(0,1,"     GAME OVER!     ");
//		game_task->state = 3;
//	}

}
// A function to implement state 3
// Ends the game when the score threshold has been met
// Prints messages and sets sound flags
void game_task_state_3_end(GameTask *game_task)
{
	// lowkey might not be needed
	// does need to reset everything but could do in above
	game_task->play_flag == 0;
	game_task->state = 1;
	game_task->score_blue = 0;
	game_task->score_red = 0;
}

