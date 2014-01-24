/*
 * display.c
 *
 * Created: 19.01.2014 14:46:55
 *  Author: snakeye
 */ 
#include <avr/io.h>

#include "display.h"
#include "max7219.h"
#include "charset_default.h"

uint8_t display_canvas[DISPLAY_SEGMENTS * 8] = {0};
uint8_t display_buffer[DISPLAY_SEGMENTS * 8] = {0};

typedef struct
{
	uint8_t reg;
	uint8_t data;
} DisplayCommand;

DisplayCommand display_cmd_buffer[DISPLAY_SEGMENTS][DISPLAY_QUEUE_LENGTH];
uint8_t display_cmd_max[DISPLAY_SEGMENTS] = {0};
uint8_t display_cmd_cur[DISPLAY_SEGMENTS] = {0};

uint8_t display_have_commands()
{
	for(uint8_t i = 0; i < DISPLAY_SEGMENTS; i++)
	{
		if (display_cmd_max[i] != display_cmd_cur[i]) 
			return 1;
	}
	return 0;
}

void display_push_cmd(uint8_t segment, uint8_t reg, uint8_t data)
{
	uint8_t pos = display_cmd_max[segment];
	display_cmd_buffer[segment][pos].reg = reg;
	display_cmd_buffer[segment][pos].data = data;
	display_cmd_max[segment] = (pos + 1) % DISPLAY_QUEUE_LENGTH;	
}

void display_execute()
{
	max7219_start();
	
	for(uint8_t i = DISPLAY_SEGMENTS - 1; i < DISPLAY_SEGMENTS; i--)
	{
		uint8_t max = display_cmd_max[i];
		uint8_t pos = display_cmd_cur[i];
		if(pos == max) 
		{
			max7219_send(MAX7219_REG_NOOP);	
			max7219_send(0);
		}
		else 
		{
			uint8_t reg = display_cmd_buffer[i][pos].reg;
			uint8_t data = display_cmd_buffer[i][pos].data; 
			max7219_send(reg);
			max7219_send(data);
			display_cmd_cur[i] = (pos + 1) % DISPLAY_QUEUE_LENGTH;
		}
	}	
	max7219_latch();
}

void display_execute_all()
{
	while(display_have_commands())
	{
		display_execute();
	}
}

void display_init()
{
	DDRC |= (1 << MAX7219_DIN) | (1 << MAX7219_CLK) | (1 << MAX7219_LOAD);
	
	for(uint8_t i = 0; i < DISPLAY_SEGMENTS; i++) {
		display_push_cmd(i, MAX7219_REG_DECODE, 0);
		display_push_cmd(i, MAX7219_REG_SCAN, 7);
		display_push_cmd(i, MAX7219_REG_TEST, 0);
		display_push_cmd(i, MAX7219_REG_SHUTDOWN, 1);
		
		for(uint8_t j = 1; j <= 8; j++) {
			display_push_cmd(i, j, 0);
		}
	}		
	
	display_execute_all();
}

void display_set_brightness(uint8_t brightness)
{
	uint8_t i;
	for(i = 0; i < DISPLAY_SEGMENTS; i++)
		display_push_cmd(i, MAX7219_REG_BRIGHTNESS, brightness);
		
	display_execute_all();
}

void display_clear_canvas()
{
	for(uint8_t i = 0; i < DISPLAY_SEGMENTS; i++) {
		for(uint8_t j = 0; j < 8; j++) {
			display_canvas[i * 8 + j] = 0x00;
		}
	}
}

void display_set_column(uint8_t col, uint8_t data)
{
	display_canvas[col] = data;
}

void display_set_pixel(uint8_t x, uint8_t y)
{
	display_canvas[x] |= (1 << y);	
}

void display_clear_pixel(uint8_t x, uint8_t y)
{
	display_canvas[x] &= ~(1 << y);
}

/**
 * @todo optimize!!!
 */
void display_draw_sprite(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t* data)
{
	for(uint8_t col = 0; col < w; col++) {
		uint8_t pixels = data[col];
		for(uint8_t row = 0; (row < h) && (row < 8); row ++) {
			if(pixels & (1 << row))
				display_set_pixel(x + col, y + row);
			else 
				display_clear_pixel(x + col, y + row);	
		}		
	}
}

void display_draw_string(uint8_t x, uint8_t y, char* str)
{
	uint8_t i = x;
	uint8_t ch;
	uint8_t char_width;
	uint16_t char_offset;
	
	for(char* p = str; *p != '\0' && i < (DISPLAY_SEGMENTS * 8); p++)
	{
		// we draw only first half of ASCII charset
		if(*p > 127)
			continue;
		
		// get character index
		ch = (uint8_t)*p;
		
		// character width
		char_width = charset_width[ch];
		if(char_width == 0)
			continue;		
		
		// character offset
		char_offset = charset_offset[ch];
		
		// draw character
		display_draw_sprite(i, y, char_width, 8, &charset_char[char_offset]);
		
		// position of the next char
		i += char_width + 1;	
	}	
}

void display_update()
{
	for(uint8_t x = 0; x < DISPLAY_SEGMENTS * 8; x++) {
		if(display_canvas[x] != display_buffer[x]) {
			uint8_t segment = (x / 8);
			uint8_t column = (x % 8) + 1; 
			display_push_cmd(segment, column, display_canvas[x]);
			display_buffer[x] = display_canvas[x];	
		}
	}	
	
	display_execute_all();
}