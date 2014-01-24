/*
* MatrixClock.c
*
* Created: 18.01.2014 21:02:58
*  Author: snakeye
*/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "lib/display.h"
#include "lib/ds1307.h"
#include "lib/brightness.h"
#include "lib/string.h"
#include "lib/usart.h"

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
	
	//
	display_init();
	
	init_adc();

	sei();
}

/**
 * 
 */
int main(void)
{
	init();

	display_set_brightness(0x03);

	//display_clear_canvas();
	//display_draw_string(3, 0, "23:00");
	//display_update();

	char str[10];

	while(1)
	{
		measure_brightness_async();
		
		str_uint16dec(str, brightness);
		
		display_clear_canvas();
		display_draw_string(0, 0, str);
		display_update();
		
		_delay_ms(500);
	}
	
	return 0;
}
