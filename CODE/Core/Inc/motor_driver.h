/**
 *  @file motor_driver.h
 *  @brief Header file for motor control functions and data structure.
 *  @author Kai De La Cruz
 *
 *  Created on: May 29, 2025
 */

#ifndef INC_MOTOR_DRIVER_H_
#define INC_MOTOR_DRIVER_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "stm32f4xx_hal.h"

/**
 * @brief Structure representing a motor object.
 *
 * Contains the timer handle and PWM channel configuration used
 * to drive the motor using two complementary outputs.
 */
typedef struct {
    TIM_HandleTypeDef *tim;  /**< Timer handle for PWM generation */
    uint32_t chan1;          /**< PWM channel 1 */
    uint32_t chan2;          /**< PWM channel 2 */
} motor_t;

/**
 * @brief Sets the motor duty cycle and direction.
 *
 * @param p_mot Pointer to the motor object
 * @param duty  Duty cycle percentage (-100 to 100)
 */
void set_duty(motor_t *p_mot, int32_t duty);

/**
 * @brief Enables the motor by applying max PWM on both channels.
 *
 * @param p_mot Pointer to the motor object
 */
void enable(motor_t *p_mot);

/**
 * @brief Disables the motor by setting both PWM channels to 0.
 *
 * @param p_mot Pointer to the motor object
 */
void disable(motor_t *p_mot);

/**
 * @brief Moves motor to a target position using proportional control.
 *
 * @param p_mot Pointer to the motor object
 * @param k_p   Proportional gain
 * @param where Target position
 * @param from  Current position
 */
void go_to(motor_t *p_mot, int k_p, int where, int from);

#endif /* INC_MOTOR_DRIVER_H_ */
