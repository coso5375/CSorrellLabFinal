/*
 * Scheduler.h
 *
 *  Created on: Sep 10, 2024
 *      Author: connorsorrell
 */

#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include "stm32f4xx_hal.h"
#include <stdint.h>

//#define APP_DELAY_FLAG_EVENT

//#define LED_TOGGLE_EVENT (1 << 0)
#define DELAY_EVENT (1 << 2)
//#define BUTTON_POLL (1 << 2)
#define GAME_TICK_EVENT (1 << 0)
#define BUTTON_EVENT    (1 << 1)
#define TOUCH_EVENT (1 << 3)
uint32_t getScheduledEvents(); // function returns a uint32_t that will be the scheduled events


void addSchedulerEvent(uint32_t event); // function adds a uint32_t that will be the event to be scheduled


void removeSchedulerEvents(uint32_t event); // function removes a uint32_t value that will be the event to be removed

void Scheduler_Tick();

#endif /* SCHEDULER_H_ */
