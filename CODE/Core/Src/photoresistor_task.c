/**
 *  @file photoresistor_task.c
 *  @brief Task logic for detecting hits using photoresistor sensors.
 *  @author Andrew Carr
 *
 *  Created on: Jun 3, 2025
 */
#include "photoresistor_task.h"
#include <stdio.h>

/**
 * @brief Executes the current state of the photoresistor task.
 *
 * Calls the appropriate function from the state list if the state
 * is within bounds. Enters infinite loop on invalid state.
 *
 * @param photoresistor_task Pointer to PhotoresistorTask instance.
 */
void photoresistor_task_run(PhotoresistorTask *photoresistor_task)
{
    if (photoresistor_task->state >= 0 &&
        photoresistor_task->state < photoresistor_task->num_states)
    {
        photoresistor_task->state_list[
            photoresistor_task->state](photoresistor_task);
    }
    else
    {
        while (1) {}
    }
}

/**
 * @brief Initializes the photoresistor task (state 0).
 *
 * Sets initial state to 1. Setup for button, laser,
 * and photoresistor has already occurred elsewhere.
 *
 * @param photoresistor_task Pointer to PhotoresistorTask instance.
 */
void photoresistor_task_state_0_init(
    PhotoresistorTask *photoresistor_task)
{
    photoresistor_task->state = 1;
}

/**
 * @brief Detects light hit based on ADC value (state 1).
 *
 * Compares current ADC value against threshold offset from zero. If
 * threshold exceeded, sets hit flag and advances to next state.
 *
 * @param photoresistor_task Pointer to PhotoresistorTask instance.
 */
void photoresistor_task_state_1_look(
    PhotoresistorTask *photoresistor_task)
{
    if ((photoresistor_task->adc_val - photoresistor_task->zero) >
        photoresistor_task->thresh)
    {
        photoresistor_task->hit_flag = 1;
        photoresistor_task->state = 2;
    }
}

/**
 * @brief Waits for score acknowledgment after a hit (state 2).
 *
 * If hit flag is cleared externally (e.g. by game_task), resets
 * to look state.
 *
 * @param photoresistor_task Pointer to PhotoresistorTask instance.
 */
void photoresistor_task_state_2_hit(
    PhotoresistorTask *photoresistor_task)
{
    if (photoresistor_task->hit_flag == 0)
    {
        photoresistor_task->state = 1;
    }
}
