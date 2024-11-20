/*
 * Button_Driver.h
 *
 *  Created on: Sep 24, 2024
 *      Author: connorsorrell
 */

#ifndef BUTTON_DRIVER_H_
#define BUTTON_DRIVER_H_
#include <stdbool.h>
#include "stm32f4xx_hal.h"

#define GREEN_LED 13
#define BUTTON_PORT_VALUE 0 //(GPIOA)
#define BUTTON_PIN_NUM 0
#define BUTTON_PRESSED 1
#define BUTTON_NOT_PRESSED 0
#define G_OFFSET 6


void Button_Init(); // function to initialize the button

void Clock_Init(); // function to initalize the clock

bool Check_Button_Status();

void Initialize_Button_Interrupt();



























#endif /* BUTTON_DRIVER_H_ */
