/*
 * adc_task.c
 *
 *  Created on: Jun 5, 2025
 *  @file adc_task.c
 *  @brief ADC task driver for managing DMA-based ADC value acquisition and assignment.
 *  @author Andrew Carr
 */

#include "adc_task.h"
#include "stm32f4xx_hal_tim.h"
#include <stdio.h>
#include "controller_task.h"
#include "photoresistor_task.h"

/**
 * @brief Buffer for DMA ADC conversions.
 *
 * Stores the most recent ADC readings for blue/red phototransistors and controllers.
 */
static uint16_t dma_buffer[4];

/**
 * @brief Executes the current state of the ADC task.
 *
 * This function checks the validity of the current state, then calls the corresponding
 * state handler from the state's function pointer list.
 *
 * @param adc_task Pointer to the ADCTask structure containing state and hardware info.
 */
void adc_task_run(ADCTask *adc_task)
{
    // Check for a valid state
    if (adc_task->state >= 0 && adc_task->state < adc_task->num_states)
    {
        // Call the appropriate state function
        adc_task->state_list[adc_task->state](adc_task);
    }
    else
    {
        while(1) {}
    }
}

/**
 * @brief Initializes the ADC task.
 *
 * Sets the task's state to 1 and starts the ADC with DMA to fill the dma_buffer.
 *
 * @param adc_task Pointer to the ADCTask structure.
 */
void adc_task_state_0_init(ADCTask *adc_task)
{
    adc_task->state = 1;
    HAL_ADC_Start_DMA(adc_task->hadc,
                      (uint32_t*)dma_buffer,
                      4);
}

/**
 * @brief Reads ADC values from DMA buffer and assigns them to respective phototransistors and controllers.
 *
 * This function maps each DMA buffer value to its corresponding sensor's `adc_val`.
 *
 * @param adc_task Pointer to the ADCTask structure.
 */
void adc_task_state_1_read(ADCTask *adc_task)
{
    adc_task->blue_photor_ptr->adc_val   = dma_buffer[0];
    adc_task->red_photor_ptr->adc_val    = dma_buffer[1];
    adc_task->blue_contr_ptr->adc_val    = dma_buffer[2];
    adc_task->red_contr_ptr->adc_val     = dma_buffer[3];
}
