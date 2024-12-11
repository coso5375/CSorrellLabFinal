/*
 * Scheduler.c
 *
 *  Created on: Sep 10, 2024
 *      Author: connorsorrell
 */

//Scheduler is used to help regulate which function gets called when
//Has a 32 bit static value, where each bit of the 32 bit corresponds to a function that needs to get called
//So, if the 0th bit is high, we will call function A. if 1st bit is high, we call function B
//Scheduled events will be added in ApplicationCode
//main.c will read the scheduled events and determine what should be called in the "super-loop"
//static uint32_t scheduledEvents;
#include "Scheduler.h"

static uint32_t ScheduledEvents;


uint32_t getScheduledEvents()
{
	return ScheduledEvents;
	// function returns a uint32_t that will be the scheduled events
}


void addSchedulerEvent(uint32_t event)
{
	// function adds a uint32_t that will be the event to be scheduled
	ScheduledEvents |= event;
}


void removeSchedulerEvents(uint32_t event)
{
	ScheduledEvents &= ~event;
}



//We will then need to add functionality that will add, remove, and return the
//contents of ‘scheduledEvents’


//Given the input argument, that should be a valid bit of a
//32-bit value. Make sure to set that bit the same way you
//would set a bit in a register




