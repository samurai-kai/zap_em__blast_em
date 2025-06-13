/**
 *  @file photoresistor_task.c
 *  @brief Task logic for hit detection using photoresistors in game loop.
 *  @author Andrew Carr
 *
 *  Created on: Jun 3, 2025
 */

#include "photoresistor_task.h"
#include <stdio.h>


/**
 * @brief Executes the current state of the photoresistor task.
 *
 * Checks that the current state index is valid and invokes the
 * corresponding state function. Traps in an infinite loop if state
 * index is invalid.
 *
 * @param photoresistor_task Pointer to PhotoresistorTask structure.
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
        while(1) {}
    }
}

/**
 * @brief Initializes the photoresistor task (state 0).
 *
 * Sets the next state to 1. Used to initialize any required
 * sensor/button components.
 *
 * @param photoresistor_task Pointer to PhotoresistorTask structure.
 */
void photoresistor_task_state_0_init(
    PhotoresistorTask *photoresistor_task)
{
    photoresistor_task->state = 1;
}

/**
 * @brief Checks for light detection and sets hit flag (state 1).
 *
 * Transitions to the hit state if the difference between the current
 * ADC value and zero calibration exceeds the threshold.
 *
 * @param photoresistor_task Pointer to PhotoresistorTask structure.
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
 * @brief Waits for score handling and clears hit flag (state 2).
 *
 * Transitions back to look state once the hit flag has been cleared
 * externally (e.g., by the game task after scoring).
 *
 * @param photoresistor_task Pointer to PhotoresistorTask structure.
 */
void photoresistor_task_state_2_hit(
    PhotoresistorTask *photoresistor_task)
{
    if (photoresistor_task->hit_flag == 0)
    {
        photoresistor_task->state = 1;
    }
}
