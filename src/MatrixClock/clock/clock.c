/*
* clock.c
*
* Created: 01.02.2014 16:34:48
*  Author: snakeye
*/
#include <avr/io.h>
#include "../lib/display.h"
#include "../lib/ds1307.h"

typedef struct {
	char digits[4];
	char next[4];
	int8_t pos[4];
	int8_t step[4];
} TimeWidget;

TimeWidget widget = {
{' ', ' ', ' ', ' '},
{' ', ' ', ' ', ' '},
{3, 9, 18, 24},
{0, 0, 0, 0},
};

/**
*
*/
void animate_clock()
{
	static uint8_t tick = 0;
	tick++;
	if(tick >= 32) {
		tick = 0;
	}
	
	//
	widget.next[0] = '0' + (ds1307_data.hour / 10);
	if(widget.next[0] == '0') {
		widget.next[0] = ' ';
	}
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
