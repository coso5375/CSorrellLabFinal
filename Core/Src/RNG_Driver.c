/*
 * RNG_Driver.c
 *
 *  Created on: Dec 2, 2024
 *      Author: connorsorrell
 */
#include "RNG_Driver.h"
#include "main.h" //DONT KNOW IF IM SUPPOSED TO DO THIS???

RNG_HandleTypeDef hrng;

void RNG_Init() // pretty sure theres a HAL function that does this for us? Cant find the proper one
{
	__HAL_RCC_RNG_CLK_ENABLE(); //enable RNG clock

	hrng.Instance = RNG; // set instance
	if (HAL_RNG_Init(&hrng) != HAL_OK)
	{
		//check if hal is OK
		Error_Handler();
	}
}

uint32_t GenerateRandomNum()
{
    uint32_t randomNumber;
    if (HAL_RNG_GenerateRandomNumber(&hrng, &randomNumber) != HAL_OK) // generate num & check if hal OK
    {

        Error_Handler();
    }

    return (randomNumber % 7) + 1; // return a random number 1 through 7
}

