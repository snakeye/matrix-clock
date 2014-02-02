/*
 * marquee.c
 *
 * Created: 01.02.2014 15:29:15
 *  Author: snakeye
 */ 
#include <avr/io.h>
#include <string.h>

#include "../lib/display.h"

#include "display_mode.h"

typedef struct {
	char str[128];
	int16_t x;
	int16_t w;
	int16_t d;
	int8_t wait;
	int8_t times;
	int8_t tick;
} Marquee;

Marquee marquee;

/**
 * 
 */
void start_marquee(const char* str, uint8_t times)
{
	strcpy(marquee.str, str);
	marquee.x = 32;
	marquee.w = display_measure_string(marquee.str);
	marquee.d = -1;
	marquee.wait = 0;
	marquee.times = times;
	marquee.tick = 0;
}

/**
 * 
 */
void animate_marquee()
{	
	if(marquee.w <= 32) {
		marquee.x = (32 - marquee.w) / 2;

		marquee.tick++;
		if(marquee.tick >= 96) {
			marquee.times = 0;
		}
	}
	else {
		if(marquee.wait == 0) {
			marquee.x += marquee.d;
			if(marquee.x <= 32 - marquee.w) {
				marquee.x = 32 - marquee.w;
				marquee.d = 1;
				marquee.wait = 30;
			}
			if(marquee.x >= 0) {
				marquee.x = 0;
				marquee.d = -1;
				marquee.wait = 30;
				marquee.times--;
			}
		}
		else {
			marquee.wait--;
		}
	}
	
	display_clear_canvas();
	display_draw_string(marquee.x, 0, marquee.str);
	
	if(marquee.times == 0) {
		display_mode = DISPLAY_MODE_CLOCK;
	}
}
