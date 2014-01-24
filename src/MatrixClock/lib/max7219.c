/*
 * max7219.c
 *
 * Created: 15.11.2013 21:43:25
 *  Author: snakeye
 */ 
#include <avr/io.h>

#include "max7219.h"

#define MAX7219_DIN_1 MAX7219_PORT |= (1 << MAX7219_DIN)
#define MAX7219_DIN_0 MAX7219_PORT &= ~(1 << MAX7219_DIN)
#define MAX7219_CLK_1 MAX7219_PORT |= (1 << MAX7219_CLK)
#define MAX7219_CLK_0 MAX7219_PORT &= ~(1 << MAX7219_CLK)
#define MAX7219_LOAD_1 MAX7219_PORT |= (1 << MAX7219_LOAD)
#define MAX7219_LOAD_0 MAX7219_PORT &= ~(1 << MAX7219_LOAD)

void max7219_send(uint8_t data)
{
	uint8_t i;
	asm("nop");
	for(i = 0; i < 8; i++) 
	{
		MAX7219_CLK_0;
		asm("nop");
		if(data & 0x80)
		{
			MAX7219_DIN_1;
		}
		else 
		{
			MAX7219_DIN_0;	
		}
		data = data << 1;
		MAX7219_CLK_1;
	}
}

void max7219_start()
{
	MAX7219_LOAD_1;
}

void max7219_latch()
{
	MAX7219_LOAD_0;
	asm("nop");
	MAX7219_LOAD_1;
}

void max7219_write(uint8_t reg, uint8_t data)
{
	max7219_start();
	max7219_send(reg);
	max7219_send(data);
	max7219_latch();
}

