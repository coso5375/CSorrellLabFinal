/*
 * RNG_Driver.h
 *
 *  Created on: Dec 2, 2024
 *      Author: connorsorrell
 */

#ifndef INC_RNG_DRIVER_H_
#define INC_RNG_DRIVER_H_

#include <stdint.h>
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_rng.h"

void RNG_Init();

uint32_t GenerateRandomNum();

#endif /* INC_RNG_DRIVER_H_ */
