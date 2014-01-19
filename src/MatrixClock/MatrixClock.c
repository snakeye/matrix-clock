/*
 * MatrixClock.c
 *
 * Created: 18.01.2014 21:02:58
 *  Author: snakeye
 */ 
#include <avr/io.h>
#include <util/delay.h>

#include "lib/display.h"
#include "lib/ds1307.h"

uint16_t get_brightness()
{
	ADMUX |= 0b111;
	ADCSRA |= (1 << ADSC);
	
	while ((ADCSRA & _BV(ADIF)) == 0x00) {	
	}
	
	return (ADCL | (ADCH << 8));
}

void init()
{
	// disable JTAG. Write twice within 4 cycles
	MCUCSR = (1<<JTD);	
	MCUCSR = (1<<JTD);
		
	//
	display_init();
	
	// init ADC
	ADCSRA = (1 << ADEN) | (1 << ADPS1) | (1 << ADPS0);
	ADMUX = (1 << REFS0);
}

int main(void)
{
	init();

	display_set_brightness(0x05);
	display_clear_canvas();

	uint8_t a[] = {
		0b01111110,
		0b00001001,
		0b00001001,
		0b00001001,
		0b01111110,
	};

	uint8_t b[] = {
		0b01111111,
		0b01001001,
		0b01001001,
		0b01001001,
		0b00110110,
	};

	uint8_t c[] = {
		0b00111110,
		0b01000001,
		0b01000001,
		0b01000001,
		0b00100010,
	};

	uint8_t d[] = {
		0b01111111,
		0b01000001,
		0b01000001,
		0b01000001,
		0b00111110,
	};

	uint8_t e[] = {
		0b01111111,
		0b01001001,
		0b01001001,
		0b01000001,
		0b01000001,
	};

	display_draw_sprite(0, 0, 5, 7, a);
	display_draw_sprite(6, 0, 5, 7, b);
	display_draw_sprite(12, 0, 5, 7, c);
	display_draw_sprite(18, 0, 5, 7, d);
	display_draw_sprite(24, 0, 5, 7, e);

	//for(uint8_t x = 0; x < 32; x++) {
	//	display_set_column(x, (x % 2) ? 0xAA : 0x55);
	//}		
			
	display_update();

    while(1)
    {
				
		_delay_ms(100);
    }
	
	return 0;
}
