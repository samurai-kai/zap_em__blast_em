/**
 * @file    encoder_task.c
 * @brief   High-level routines for initializing and reading a rotary encoder.
 *
 * Created on: Jun 7, 2025
 * @author  Andrew Carr and Kai De La Cruz
 */

#include "encoder_driver.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "stm32f4xx_hal.h"

// Stall detection parameters
#define STALL_THRESHOLD   5    /**< ticks change below this is considered stalled */
#define STALL_COUNT      20    /**< consecutive samples to confirm stall */
#define STALL_DELAY_MS   10    /**< sampling interval in milliseconds */

/**
 * @brief  Initialize encoder state.
 *
 * Captures the current hardware timer counter as the zero reference,
 * and clears both the cumulative tick count and the last-read snapshot.
 *
 * @param  p_enc  Pointer to an encoder_t instance.
 */
void setup_encoder(encoder_t *p_enc){
    p_enc->zero       = __HAL_TIM_GET_COUNTER(p_enc->htim);
    p_enc->ticks      = 0;
    p_enc->last_ticks = 0;
}

/**
 * @brief  Read and accumulate encoder ticks since last call.
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
 *
 * @param  p_enc  Pointer to an encoder_t instance.
 */
void read_encoder(encoder_t *p_enc)
{
    /* Compute signed tick count relative to zero */
    int32_t current_ticks = (int32_t)(__HAL_TIM_GET_COUNTER(p_enc->htim))
                            - (int32_t)(p_enc->zero);
    int32_t delta = current_ticks - p_enc->last_ticks;

    /* Handle wrap-around if delta jumps more than half-range */
    if (delta > (p_enc->ar + 1) / 2) {
        delta -= (p_enc->ar + 1);
    }
    else if (delta < -(p_enc->ar + 1) / 2) {
        delta += (p_enc->ar + 1);
    }

    /* Update state */
    p_enc->last_ticks = current_ticks;
    p_enc->ticks     += delta;
}

/**
 * @brief  Waits until the encoder motion stalls.
 *
 * Repeatedly samples encoder ticks and detects stall if no significant
 * motion occurs over a defined interval.
 *
 * @param  p_enc  Pointer to an encoder_t instance.
 */
void wait_for_stall(encoder_t *p_enc)
{
    int32_t prev_ticks = p_enc->ticks;
    int stable_count = 0;
    while (stable_count < STALL_COUNT) {
        // Update encoder ticks
        read_encoder(p_enc);
        int32_t delta = p_enc->ticks - prev_ticks;
        if (abs(delta) < STALL_THRESHOLD) {
            stable_count++;
        } else {
            stable_count = 0;
        }
        prev_ticks = p_enc->ticks;
        HAL_Delay(STALL_DELAY_MS);
    }
}
