/*
 * Button_Driver.c
 *
 *  Created on: Sep 24, 2024
 *      Author: connorsorrell
 */
#include "Button_Driver.h"


void Button_Init() // function to initialize the button
{
	GPIO_InitTypeDef localStruct = {0};


	//configuring the pin for button
	localStruct.Pin = GPIO_PIN_0; // button pin #
	localStruct.Mode = GPIO_MODE_INPUT; //NEEDS INPUT MODE
	localStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	localStruct.Pull = GPIO_NOPULL; // IDK ABOUT THIS
	localStruct.Alternate = 0;

	Clock_Init();
	HAL_GPIO_Init(GPIOA, &localStruct);



}

void Clock_Init() // function to initalize the clock
{
	__HAL_RCC_GPIOA_CLK_ENABLE(); //enable gpioa clock for button

}


bool Check_Button_Status()
{
	uint8_t pin = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0); // find the pin of button (0 = A_OFFSET)

	if(pin == BUTTON_PRESSED)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Initialize_Button_Interrupt()
{
	GPIO_InitTypeDef localStruct = {0};
	localStruct.Pin = GPIO_PIN_0;
	localStruct.Mode = GPIO_MODE_IT_RISING;
	localStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	localStruct.Pull = GPIO_NOPULL;
	Clock_Init();

	HAL_GPIO_Init(GPIOA, &localStruct);

	HAL_NVIC_EnableIRQ(G_OFFSET); // 6
}
