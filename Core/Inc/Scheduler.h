/*
 * Scheduler.h
 *
 *  Created on: Sep 10, 2024
 *      Author: connorsorrell
 */

#ifndef SCHEDULER_H_
#define SCHEDULER_H_

//#include "stm32f4xx_hal.h"
#include <stdint.h>

//#define APP_DELAY_FLAG_EVENT

//#define LED_TOGGLE_EVENT (1 << 0)

//#define BUTTON_POLL (1 << 2)
#define GAME_TICK_EVENT (1 << 0)
#define BUTTON_EVENT    (1 << 1)
#define DELAY_EVENT (1 << 2)
#define TOUCH_EVENT (1 << 3)
#define ROTATE_EVENT (1 << 4)
#define MOVE_RIGHT_EVENT (1 << 5)
#define MOVE_LEFT_EVENT (1 << 6)
#define BUTTON_HOLD_EVENT (1 << 7)
#define START_EVENT (1 << 8)

uint32_t getScheduledEvents(); // function returns a uint32_t that will be the scheduled events


void addSchedulerEvent(uint32_t event); // function adds a uint32_t that will be the event to be scheduled


void removeSchedulerEvents(uint32_t event); // function removes a uint32_t value that will be the event to be removed



#endif /* SCHEDULER_H_ */
