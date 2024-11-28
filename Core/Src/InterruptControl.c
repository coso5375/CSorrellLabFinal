/*
 * InterruptControl.c
 *
 *  Created on: Nov 20, 2024
 *      Author: connorsorrell
 */


#include "InterruptControl.h"

void enableInterrupt(uint8_t IRQNumber)
{
	if(IRQNumber < 32)
	{
		*NVIC_ISER0 |= (1<<IRQNumber);
	}
	else
	{
		*NVIC_ISER1 |= (1<< (IRQNumber % 32));
	}
}
void disableInterrupt(uint8_t IRQNumber)
{
	if(IRQNumber < 32)
	{
		*NVIC_ICER0 |= (1<<IRQNumber);
	}
	else
	{
		*NVIC_ICER1 |= (1<< (IRQNumber %32));
	}
}
void clearInterrupt (uint8_t IRQNumber)
{
	if(IRQNumber < 32)
	{
		*NVIC_ICPR0 |= (1<<IRQNumber);
	}
	else
	{
		*NVIC_ICPR1 |= (1<< (IRQNumber %32));
	}
}
void setInterrupt (uint8_t IRQNumber)
{
	if(IRQNumber < 32)
	{
		*NVIC_ISPR0 |= (1<<IRQNumber);
	}
	else
	{
		*NVIC_ISPR1 |= (1<< (IRQNumber % 32));
	}
}

void clearEXTIInterrupt (uint8_t pinNumber)
{
	if(pinNumber < 22)
	{
		EXTI ->PR |= (1<<pinNumber);
	}
	else
	{
		EXTI ->PR |= (1<< (pinNumber% 22));
	}
}
