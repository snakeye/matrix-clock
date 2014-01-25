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

#include "lib/settings.h"

#define MAX_BRIGHTNESS 0x0f

typedef enum {
	DISPLAY_MODE_TEST,
	DISPLAY_MODE_TIME,
	DISPLAY_MODE_DATE
} DisplayMode;

/**
* Initialize
*/
void init()
{
	// disable JTAG. Write twice within 4 cycles
	MCUCSR = (1<<JTD);
	MCUCSR = (1<<JTD);
	
	// initialize display
	display_init();
	
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

void process_terminal()
{
	char response[64];
	
	// hello
	if(strcmp(usart_command, "Hello\r\n") == 0) {
		usart_transmit_async("Hello\r\n");
		return;
	}
	
	if(strcmp(usart_command, "Time?\r\n") == 0) {
		char buffer[10];
		str_uint16dec(response, ds1307_addr[2]);
		
		stradd(response, ":");
		
		str_uint16dec(buffer, ds1307_addr[1]);
		stradd(response, buffer);
		
		stradd(response, ":");
		
		str_uint16dec(buffer, ds1307_addr[0]);
		stradd(response, buffer);
		
		//
		stradd(response, "\r\n");
		usart_transmit_async(response);
		
		return;
	}
	
	// asked brightness
	if(strcmp(usart_command, "Light?\r\n") == 0){
		str_uint16dec(response, brightness);
		
		//
		stradd(response, "\r\n");
		usart_transmit_async(response);
		
		return;
	}
	
	usart_transmit_async("Error\r\n");
}

/**
*
*/
int main(void)
{
	// initialize everything
	init();

	// initial settings
	load_settings();
	
	// apply loaded settings
	apply_settings();

	display_clear_canvas();
	display_draw_string(0, 0, "Hello");
	display_update();

	while(1)
	{
		// process inputs
		if(usart_has_command) {
			process_terminal();
			usart_has_command = 0;
		}
		
		
		if(settings.brightness_auto) {
			measure_brightness_async();
		}
		
		// set display brightness
		if(settings.brightness_auto) {
			uint8_t b = (brightness * (settings.brightness_max - settings.brightness_min)) / 1024 + settings.brightness_min;
			display_set_brightness(b);
		}
		
		_delay_ms(40);
	}
	
	return 0;
}
