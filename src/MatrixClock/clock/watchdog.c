/*
* watchdog.c
*
* Created: 03.05.2015 13:19:48
*  Author: snakeye
*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>

#include "watchdog.h"

void watchdog_init()
{
	//reset watchdog
	wdt_reset();
	//set up WDT interrupt
	WDTCR = (1<<WDTOE)|(1<<WDE)|(1<<WDP2)|(1<<WDP1)|(1<<WDP0);
}
