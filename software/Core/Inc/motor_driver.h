/**
 *  @file motor_driver.h
 *  @brief Header file for motor control structure and interface functions.
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
 * @brief Motor object data structure.
 *
 * Holds the timer handle and channel configuration for bidirectional PWM.
 */
typedef struct {
    TIM_HandleTypeDef *tim;  /**< Timer handle for PWM */
    uint32_t chan1;          /**< PWM channel 1 */
    uint32_t chan2;          /**< PWM channel 2 */
} motor_t;

/**
 * @brief Sets the duty cycle and direction of the motor.
 *
 * @param p_mot Pointer to motor object
 * @param duty  Duty cycle (-100 to 100)
 */
void set_duty(motor_t *p_mot, int32_t duty);

/**
 * @brief Enables the motor by setting both channels to maximum.
 *
 * @param p_mot Pointer to motor object
 */
void enable(motor_t *p_mot);

/**
 * @brief Disables the motor by setting both channels to zero.
 *
 * @param p_mot Pointer to motor object
 */
void disable(motor_t *p_mot);

/**
 * @brief Uses proportional control to move motor to a target.
 *
 * @param p_mot Pointer to motor object
 * @param k_p   Proportional gain
 * @param where Target position
 * @param from  Current position
 */
void go_to(motor_t *p_mot, int k_p, int where, int from);

#endif /* INC_MOTOR_DRIVER_H_ */
