/**
 * @file    encoder_task.c
 * @brief   High-level routines for initializing and reading a rotary encoder.
 *
 * Created on: Jun 7, 2025
 * Author:     Andrew Carr and Kai De La Cruz
 */

#include "encoder_driver.h"
#include <stdio.h>
#include "stm32f4xx_hal.h"

/**
 * @brief  Initialize encoder state.
 * @param  p_enc  Pointer to an encoder_t instance.
 *
 * Captures the current hardware timer counter as the zero reference,
 * and clears both the cumulative tick count and the last-read snapshot.
 */
void setup_encoder(encoder_t *p_enc){
    p_enc->zero       = __HAL_TIM_GET_COUNTER(p_enc->htim);
    p_enc->ticks      = 0;
    p_enc->last_ticks = 0;
}

/**
 * @brief  Read and accumulate encoder ticks since last call.
 * @param  p_enc  Pointer to an encoder_t instance.
 *
 * Computes the difference between the current timer count and the zero
 * reference, applies wrap-around correction based on the encoder’s
 * auto-reload value (p_enc->ar), and updates:
 *   - last_ticks: snapshot of this read
 *   - ticks:      cumulative total (signed)
 *
 * Wrap-around logic:
 *   If the delta exceeds half the auto-reload range, it is adjusted
 *   by ±(ar+1) to account for rollover.
 */
void read_encoder(encoder_t *p_enc){

    int32_t current_ticks = (int32_t)(__HAL_TIM_GET_COUNTER(p_enc->htim))
                            - (int32_t)(p_enc->zero);
    int32_t delta = current_ticks - p_enc->last_ticks;

    if (delta > (p_enc->ar + 1)/2){
        delta -= p_enc->ar + 1;
    }
    else if (delta < (-(p_enc->ar + 1)/2)){
        delta += p_enc->ar + 1;
    }

    p_enc->last_ticks = current_ticks;
    p_enc->ticks     += delta;
}
