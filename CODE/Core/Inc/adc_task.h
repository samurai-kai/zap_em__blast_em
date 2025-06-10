/**
 *  @file adc_task.h
 *  @brief Header file for the ADC task system
 *  @author Andrew Carr
 *
 *  Created on: Jun 5, 2025
 *
 */

#ifndef INC_ADC_TASK_H_
#define INC_ADC_TASK_H_

#include <stdlib.h>
#include <stdio.h>
#include "stm32f4xx_hal.h"
#include <stdint.h>

/**
 * @brief Forward declaration for ControllerTask.
 */
typedef struct ControllerTask ControllerTask;

/**
 * @brief Forward declaration for PhotoresistorTask.
 */
typedef struct PhotoresistorTask PhotoresistorTask;

/**
 * @brief Forward declaration for ADCTask.
 */
typedef struct ADCTask ADCTask;

/**
 * @brief Function pointer type for ADC state functions.
 *
 * Each state function must take a pointer to ADCTask as its argument.
 */
typedef void (*adc_fcn_t)(ADCTask *adc_task);

/**
 * @brief Structure that represents the ADC task state and configuration.
 *
 * This structure holds all the contextual information and function pointers
 * necessary to run ADC operations within a task-based state machine.
 */
struct ADCTask
{
    int32_t              state;            /**< Current state index */
    int32_t              num_states;       /**< Total number of states */
    ControllerTask       *red_contr_ptr;   /**< Pointer to red controller task object*/
    ControllerTask       *blue_contr_ptr;  /**< Pointer to blue controller task object*/
    PhotoresistorTask    *red_photor_ptr;  /**< Pointer to red photo resistor task object*/
    PhotoresistorTask    *blue_photor_ptr; /**< Pointer to blue photo resistor task object*/
    ADC_HandleTypeDef    *hadc;            /**< Pointer to the ADC channel handle */
    adc_fcn_t            state_list[];     /**< Array of function pointers for states */
};

/**
 * @brief Initializes the ADC task (state 0).
 *
 * @param adc_task Pointer to the ADCTask instance.
 */
void adc_task_state_0_init(ADCTask *adc_task);

/**
 * @brief Executes the current ADC task state.
 *
 * @param adc_task Pointer to the ADCTask instance.
 */
void adc_task_run(ADCTask *adc_task);

/**
 * @brief Reads and assigns values from the ADC DMA buffer (state 1).
 *
 * @param adc_task Pointer to the ADCTask instance.
 */
void adc_task_state_1_read(ADCTask *adc_task);

#endif /* INC_ADC_TASK_H_ */
