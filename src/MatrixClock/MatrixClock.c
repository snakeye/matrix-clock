/*
* MatrixClock.c
*
* Created: 18.01.2014 21:02:58
*  Author: snakeye
*/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include <string.h>

#include "lib/display.h"
#include "lib/ds1307.h"
#include "lib/brightness.h"
#include "lib/string.h"
#include "lib/usart.h"
#include "lib/buttons.h"
#include "lib/ds1307.h"
#include "lib/terminal.h"

#include "lib/settings.h"

#define MAX_BRIGHTNESS 0x0f

typedef enum {
	DISPLAY_MODE_TEST,
	DISPLAY_MODE_TIME,
	DISPLAY_MODE_DATE_SHORT,
	DISPLAY_MODE_DATE_MED,
	DISPLAY_MODE_WEEKDAY,
} DisplayMode;

DisplayMode display_mode = DISPLAY_MODE_TIME;

/**
* Initialize
*/
void init()
{
	// initialize ADC
	adc_init();
	
	// initialize buttons
	buttons_init();
	
	// init USART
	usart_init(38400);
	// start receiving commands
	usart_receive_async();

	//
	//ds1307_reset();
	
	// enable interrupts
	sei();
}

void on_button_click(uint8_t button)
{
}

void on_button_hold(uint8_t button)
{
}

void on_button_release(uint8_t button)
{
}

void animate_time()
{
	static uint8_t min = 0;
	static uint8_t tick = 0;
	char buf[10];
	
	tick++;
	if(tick >= 30) {
		min = (min + 1) % 60;
		tick = 0;
	}
		
	buf[0] = '0';
	buf[1] = '0';
	buf[2] = ':';
	buf[3] = '0' + (min / 10);
	buf[4] = '0' + (min % 10);
	buf[5] = 0;
	
	display_clear_canvas();
	display_draw_string(3, 0, buf);
}

void animation_tick()
{
	static uint8_t tick = 0;
	tick++;
	if(tick >= 30) {

		switch(display_mode) {
			case DISPLAY_MODE_TEST:
				break;
			case DISPLAY_MODE_TIME:
				animate_time(); 
				break;
		}

		display_update();
		tick = 0;
	}
}

/**
*
*/
int main(void)
{
	// initialize display
	display_init();

	// Welcome string	
	display_clear_canvas();
	display_draw_string(0, 0, "Hello!");
	display_update();

	// initialize everything
	init();

	// initial settings
	load_settings();
	
	// apply loaded settings
	apply_settings();
	
	while(1)
	{
		// process inputs
		if(usart_has_command) {
			process_terminal();
			usart_has_command = 0;
		}

		//
		buttons_tick();

		//
		brightness_tick();

		//
		animation_tick();

		//		
		_delay_ms(1);
	}
	
	return 0;
}
