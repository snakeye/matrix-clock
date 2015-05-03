/*
* brightness.c
*
* Created: 24.01.2014 22:59:26
*  Author: snakeye
*/
#include <avr/io.h>
#include <avr/interrupt.h>

#include "settings.h"
#include "../lib/display.h"

#define HISTORY_LENGTH 20

uint16_t brightness_history[HISTORY_LENGTH] = {0};
uint8_t brightness_ptr = 0;

uint16_t brightness = 0x8;

/**
* ADC Interrupt handler
*/
ISR(ADC_vect)
{
	uint16_t br = (ADCL | (ADCH << 8));
	brightness_history[brightness_ptr] = br;
	brightness_ptr = (brightness_ptr + 1) % HISTORY_LENGTH;
	
	br = 0;
	for(uint8_t i = 0; i < HISTORY_LENGTH; i++) {
		br += brightness_history[i];		
	}
	
	// average
	brightness = br / HISTORY_LENGTH;
}

void adc_init()
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

/**
 * 
 */
uint16_t measure_brightness_sync()
{
	ADCSRA |= (1 << ADSC);
	
	// wait for conversion finished
	//while(){
	//}
	
	//
	brightness = (ADCL | (ADCH << 8));

	return brightness;
}

/**
 * 
 */
void brightness_tick()
{
	static uint8_t tick = 0;

	tick++;
	if(tick >= 250) {
		tick = 0;
		
		if(settings.brightness_auto) {
			//
			uint8_t b = (brightness * (settings.brightness_max - settings.brightness_min)) / 1024 + settings.brightness_min;
			display_set_brightness(b);
			
			//
			measure_brightness_async();
		}
	}
}