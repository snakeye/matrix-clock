/*
 * marquee.c
 *
 * Created: 01.02.2014 15:29:15
 *  Author: snakeye
 */ 
#include <avr/io.h>
#include <string.h>

#include "../lib/display.h"

typedef struct {
	char str[128];
	int16_t x;
	int16_t w;
	int16_t d;
	int8_t wait;
} Marquee;

Marquee marquee;

/**
 * 
 */
void start_marquee(const char* str)
{
	strcpy(marquee.str, str);
	marquee.x = 32;
	marquee.w = display_measure_string(marquee.str);
	marquee.d = -1;
	marquee.wait = 0;
}

/**
 * 
 */
void animate_marquee()
{	
	if(marquee.w <= 32) {
		marquee.x = (32 - marquee.w) / 2;
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
			}
		}
		else {
			marquee.wait--;
		}
	}
	
	display_clear_canvas();
	display_draw_string(marquee.x, 0, marquee.str);
}
