/*
 * ErrorHandling.c
 *
 *  Created on: Oct 31, 2024
 *      Author: connorsorrell
 */
#include "ErrorHandling.h"


void APPLICATION_ASSERT(bool Arg)
{
	if(Arg == false)
	{
		while(1)//infinite loop
		{

		}
	}
}
