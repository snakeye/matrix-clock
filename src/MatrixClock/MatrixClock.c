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
	DISPLAY_MODE_WELCOME,
	DISPLAY_MODE_TIME,
	DISPLAY_MODE_DATE_SHORT,
	DISPLAY_MODE_DATE_MED,
	DISPLAY_MODE_WEEKDAY,
	DISPLAY_MODE_SETTINGS,
	DISPLAY_MODE_TEST
} DisplayMode;

DisplayMode display_mode = DISPLAY_MODE_TIME;

/**
* Initialize
*/
void init()
{
	// enable interrupts
	sei();

	// initialize ADC
	adc_init();
	
	// initialize buttons
	buttons_init();
	
	// init USART
	usart_init(38400);
	// start receiving commands
	usart_receive_async();

	//
	i2c_init();

	// reset clock
	ds1307_reset();
	ds1307_read();
}

uint8_t settings_mode;

/**
 * 
 */
void on_button_click(uint8_t button)
{
	if(button == 0) {
		if(display_mode == DISPLAY_MODE_SETTINGS) {
			switch(settings_mode) {
				case 0:
				ds1307_data.hour += 1;
				if(ds1307_data.hour > 23) ds1307_data.hour = 0;
				ds1307_write();
				break;
				case 1:
				ds1307_data.minute += 1;
				if(ds1307_data.minute > 59) ds1307_data.minute = 0;
				ds1307_write();
				break;
			}
		}
		else {
			display_mode = DISPLAY_MODE_DATE_SHORT;
		}
	}
	
	if(button == 1) {
		if(display_mode == DISPLAY_MODE_SETTINGS) {
			switch(settings_mode) {
				case 0:
				ds1307_data.hour -= 1;
				if(ds1307_data.hour > 23) ds1307_data.hour = 23;
				ds1307_write();
				break;
				case 1:
				ds1307_data.minute -= 1;
				if(ds1307_data.minute > 59) ds1307_data.minute = 59;
				ds1307_write();
				break;
			}
		}
		else {
			display_mode = DISPLAY_MODE_WEEKDAY;
		}
	}
	
	if(button == 2) {
		if(display_mode == DISPLAY_MODE_SETTINGS) {
			settings_mode++;
			if(settings_mode >= 2) {
				display_mode = DISPLAY_MODE_TIME;
			}
		}
		else {
			display_mode = DISPLAY_MODE_TIME;
		}
	}
}

/**
 * 
 */
void on_button_hold(uint8_t button)
{
	if(button == 2) {
		display_mode = DISPLAY_MODE_SETTINGS;
		settings_mode = 0;
	}
}

/**
 * 
 */
void on_button_release(uint8_t button)
{
}


typedef struct {
	char digits[4];
	char pos[4];
	char next[4];
	char step[4];
} TimeWidget;

TimeWidget widget = {
{' ', ' ', ' ', ' '},
{3, 9, 18, 24},
{' ', ' ', ' ', ' '},
{0, 0, 0, 0},
};

/**
 * 
 */
void animate_welcome()
{
	static uint8_t tick = 0;
	
	if(tick <= 8) {
		display_clear_canvas();
		display_draw_string(3 , 8 - tick, "Hello!");
	}
	
	tick++;
	if(tick >= 32) {
		tick = 0;
		display_mode = DISPLAY_MODE_TIME;
	}
}

/**
 * 
 */
void animate_test()
{
	static uint8_t tick = 0;
	
	if(tick == 0) {
		display_test_mode();
	}
	
	tick++;
	if(tick > 64) {
		tick = 0;
		display_mode = DISPLAY_MODE_TIME;
	}
}

/**
 * 
 */
void animate_time()
{
	static uint8_t tick = 0;
	tick++;
	if(tick >= 32) {
		tick = 0;
	}
	
	//
	widget.next[0] = '0' + (ds1307_data.hour / 10);
	widget.next[1] = '0' + (ds1307_data.hour % 10);
	
	widget.next[2] = '0' + (ds1307_data.minute / 10);
	widget.next[3] = '0' + (ds1307_data.minute % 10);

	// start animations where needed
	for(uint8_t i = 0; i <  4; i++) {
		if(widget.digits[i] != widget.next[i] && widget.step[i] == 0) {
			widget.step[i] = 8;
		}
	}
	
	// draw widget
	display_clear_canvas();
	
	if(tick > 16) {
		display_draw_char(15, 0, ':');
	}
	
	// draw digits
	for(uint8_t i = 0; i < 4; i++) {
		if(widget.digits[i] != widget.next[i]) {
			display_draw_char(widget.pos[i], -8 + widget.step[i], widget.digits[i]);
			display_draw_char(widget.pos[i], widget.step[i], widget.next[i]);
		}
		else {
			display_draw_char(widget.pos[i], 0, widget.digits[i]);
		}
		
		// update animation step
		if(widget.step[i] > 0) {
			widget.step[i]--;
			if(widget.step[i] == 0){
				widget.digits[i] = widget.next[i];
			}
		}
	}
}

/**
 * 
 */
void animate_date_short()
{
	static uint8_t tick = 0;
	
	if(tick == 0) {
		char buffer[6];
		
		buffer[0] = '0' + (ds1307_data.day / 10);
		buffer[1] = '0' + (ds1307_data.day % 10);
		buffer[2] = '.';
		buffer[3] = '0' + (ds1307_data.month / 10);
		buffer[4] = '0' + (ds1307_data.month % 10);
		buffer[5] = '\0';

		display_clear_canvas();
		display_draw_string(3, 0, buffer);
	}
	
	tick++;
	if(tick >= 64) {
		display_mode = DISPLAY_MODE_TIME;
		tick = 0;
	}
}

char* month_names[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

/**
 * 
 */
void animate_date_med()
{
	static uint8_t tick = 0;
	
	if(tick == 0) {
		char buffer[10];
		
		buffer[0] = '0' + (ds1307_data.day / 10);
		buffer[1] = '0' + (ds1307_data.day % 10);

		stradd(buffer, month_names[ds1307_data.month - 1]);

		display_clear_canvas();
		display_draw_string(1, 0, buffer);
	}
	
	tick++;
	if(tick >= 64) {
		display_mode = DISPLAY_MODE_TIME;
		tick = 0;
	}
}

char* weekdays[7] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};

/**
 * 
 */
void animate_weekday()
{
	static uint8_t tick = 0;
	
	if(tick == 0) {
		char buffer[10];
		
		strcpy(buffer, weekdays[ds1307_data.weekday - 1]);

		display_clear_canvas();
		display_draw_string(7, 0, buffer);
	}
	
	tick++;
	if(tick >= 64) {
		display_mode = DISPLAY_MODE_TIME;
		tick = 0;
	}
}

/**
 * 
 */
void animate_settings()
{
	static uint8_t tick = 0;
	tick++;
	
	static char buffer[10];
	
	buffer[0] = '0' + (ds1307_data.hour / 10);
	buffer[1] = '0' + (ds1307_data.hour % 10);
	buffer[2] = ':';
	buffer[3] = '0' + (ds1307_data.minute / 10);
	buffer[4] = '0' + (ds1307_data.minute % 10);	
	
	if(tick < 16) {
		if(settings_mode == 0) {
			buffer[0] = ' ';
			buffer[1] = ' ';
		}
		if(settings_mode == 1) {
			buffer[3] = ' ';
			buffer[4] = ' ';
		}
	}	
	
	display_clear_canvas();
	display_draw_string(3, 0, buffer);
	
	if(tick >= 32) {
		tick = 0;
	}
}

/**
 * 
 */
void animation_tick()
{
	static uint8_t tick = 0;

	tick++;
	if(tick >= 30) {
		tick = 0;

		switch(display_mode) {
			case DISPLAY_MODE_WELCOME:
			animate_welcome();
			case DISPLAY_MODE_TEST:
			animate_test();
			break;
			case DISPLAY_MODE_TIME:
			animate_time();
			break;
			case DISPLAY_MODE_DATE_SHORT:
			animate_date_short();
			break;
			case DISPLAY_MODE_DATE_MED:
			animate_date_med();
			break;
			case DISPLAY_MODE_WEEKDAY:
			animate_weekday();
			break;
			case DISPLAY_MODE_SETTINGS:
			animate_settings();
			break;
		}
		display_update();
	}
}

void on_date_changed()
{
	if(ds1307_data.hour == 0 && ds1307_data.minute == 0) {
		display_mode = DISPLAY_MODE_DATE_SHORT;
	}
}

void on_time_changed()
{
	
}

/**
*
*/
int main(void)
{
	// initialize display
	display_init();

	// initialize everything
	init();

	// initial settings
	load_settings();
	
	// apply loaded settings
	apply_settings();
	
	display_mode = DISPLAY_MODE_WELCOME;
	
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
		ds1307_tick();

		//
		animation_tick();

		//
		_delay_ms(1);
	}
	
	return 0;
}
