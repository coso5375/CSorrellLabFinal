/*
 * InterruptControl.h
 *
 *  Created on: Nov 20, 2024
 *      Author: connorsorrell
 */

#ifndef INC_INTERRUPTCONTROL_H_
#define INC_INTERRUPTCONTROL_H_

#include <stdint.h>
#include "stm32f429xx.h"


#define EXTI_IRQ_NUMBER 6

#define TIM2_IRQ_NUMBER 28

#define TIM5_IRQ_NUMBER 50

#define NVIC_ISER0 ((volatile uint32_t*) 0xE000E100)
#define NVIC_ICER0 ((volatile uint32_t*) 0xE000E180)
#define NVIC_ISPR0 ((volatile uint32_t*) 0xE000E200)
#define NVIC_ICPR0 ((volatile uint32_t*) 0xE000E280)
#define NVIC_IABR0 ((volatile uint32_t*) 0xE000E300)
#define NVIC_IPR0  ((volatile uint32_t*) 0xE000E400)

#define NVIC_ISER1 ((volatile uint32_t*) 0xE000E104)
#define NVIC_ICER1 ((volatile uint32_t*) 0xE000E184)
#define NVIC_ISPR1 ((volatile uint32_t*) 0xE000E204)
#define NVIC_ICPR1 ((volatile uint32_t*) 0xE000E284)
#define NVIC_IABR1 ((volatile uint32_t*) 0xE000E304)
#define NVIC_IPR1  ((volatile uint32_t*) 0xE000E404)


void enableInterrupt(uint8_t IRQNumber);
void disableInterrupt(uint8_t IRQNumber);
void clearInterrupt (uint8_t IRQNumber);
void setInterrupt (uint8_t IRQNumber);


void clearEXTIInterrupt (uint8_t pinNumber);

#endif /* INC_INTERRUPTCONTROL_H_ */
