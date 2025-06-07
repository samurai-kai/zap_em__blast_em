/*
 * encoder_task.h
 *
 *  Created on: Jun 7, 2025
 *      Author: andrewcarr
 */

#ifndef INC_ENCODER_DRIVER_H_
#define INC_ENCODER_DRIVER_H_

#include <stdint.h> // Allows use of standard integer types
#include "stm32f4xx_hal.h"  // or your STM32 family


// A datatype for a structure to hold task configuration and state.
// Additional fields can be added as desired.
typedef struct
{
    uint32_t				zero;
    uint32_t				ar;
    int32_t					ticks;
    uint32_t				last_ticks;
    int32_t					range;
    TIM_HandleTypeDef 		*htim;
} encoder_t;


void setup_encoder(encoder_t *p_enc);
void read_encoder(encoder_t *p_enc);

#endif /* INC_ENCODER_DRIVER_H_ */
