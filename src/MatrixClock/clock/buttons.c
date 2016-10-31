/*
* buttons.c
*
* Created: 24.01.2014 23:09:28
*  Author: snakeye
*/
#include <avr/io.h>

#include "buttons.h"

#define BUTTONS_NUMBER 3

uint16_t buttons_hold_time[BUTTONS_NUMBER];
uint8_t buttons_hold_fired[BUTTONS_NUMBER];
uint8_t buttons_status[BUTTONS_NUMBER];

void buttons_init()
{
	// port D inputs
	DDRD &= ~((1 << PORTD5) | (1 << PORTD6) | (1 << PORTD7));
	// port D pull-up resistors
	PORTD |= (1 << PORTD5) | (1 << PORTD6) | (1 << PORTD7);

	for(uint8_t i = 0; i < BUTTONS_NUMBER; i++) {
		buttons_hold_time[i] = 0;
	}
}

void buttons_tick()
{
	buttons_status[0] = (PIND & (1 << PORTD7)) == 0;
	buttons_status[1] = (PIND & (1 << PORTD6)) == 0;
	buttons_status[2] = (PIND & (1 << PORTD5)) == 0;

	for(uint8_t i = 0; i < BUTTONS_NUMBER; i++) {
		if(buttons_status[i]) {
			buttons_hold_time[i]++;
			
			if(buttons_hold_time[i] >= BUTTON_HOLD_TIME && !buttons_hold_fired[i]){
				on_button_hold(i);
				buttons_hold_fired[i] = 1;
			}
		}
		else {
			if(buttons_hold_time[i] >= BUTTON_HOLD_TIME) {
				on_button_release(i);
				buttons_hold_time[i] = 0;
				buttons_hold_fired[i] = 0;
			}
			else if(buttons_hold_time[i] >= BUTTON_CLICK_TIME) {
				on_button_click(i);
				buttons_hold_time[i] = 0;
			}
		}
	}
}
