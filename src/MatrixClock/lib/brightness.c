/*
* brightness.c
*
* Created: 24.01.2014 22:59:26
*  Author: snakeye
*/
#include <avr/io.h>
#include <avr/interrupt.h>

uint16_t brightness;

/**
* ADC Interrupt handler
*/
ISR(ADC_vect)
{
	brightness = (ADCL | (ADCH << 8));
}

void init_adc()
{
	// init ADC
	// VCC with capacitor on AREF
	// ADC7 as source
	ADMUX = (1 << REFS0) | (0b111);
	// ADC enable
	// prescale = 128
	// interrupt enable
	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0)  | (1 << ADIE);
}

/**
* Start ADC conversion
*/
void measure_brightness_async()
{
	ADCSRA |= (1 << ADSC);
}

uint16_t measure_brightness_sync()
{
	ADCSRA |= (1 << ADSC);


	
	brightness = (ADCL | (ADCH << 8));

	return brightness;
}