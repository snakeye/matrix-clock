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

#define MAX_BRIGHTNESS 0x0f

/**
 * ADC Interrupt handler
 */
ISR(ADC_vect)
{
	uint16_t adc = (ADCL | (ADCH << 8));
	uint8_t brightness = (adc * MAX_BRIGHTNESS) / 1024;
	
	display_set_brightness(brightness);
}

/**
 * Start ADC conversion
 */
void update_brightness()
{
	ADCSRA |= (1 << ADSC);
}

void init()
{
	// disable JTAG. Write twice within 4 cycles
	MCUCSR = (1<<JTD);	
	MCUCSR = (1<<JTD);
		
	//
	display_init();
	
	// init ADC
	// VCC with capacitor on AREF
	// ADC7 as source
	ADMUX = (1 << REFS0) | (0b111);
	// ADC enable
	// prescale = 128
	// interrupt enable
	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0)  | (1 << ADIE);

	sei();
}

int main(void)
{
	init();

	display_set_brightness(0x03);

	display_clear_canvas();
	display_draw_string(2, 0, "a=b c");
	display_update();

    while(1)
    {
		update_brightness();
		
		_delay_ms(50);
    }
	
	return 0;
}
