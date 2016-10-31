/*
* settings.c
*
* Created: 24.01.2014 23:08:08
*  Author: snakeye
*/
#include <avr/io.h>

#include "settings.h"
#include "../lib/display.h"

Settings settings;

/**
 * 
 */
void default_settings()
{
	settings.tzOffset = 0;
	settings.brightness = 0x00;
	settings.brightness_max = 0x0f;
}

/**
 * 
 */
void load_settings()
{
	default_settings();
}

/**
 * 
 */
void save_settings()
{
	
}

/**
 * 
 */
void apply_settings()
{
	display_set_brightness(settings.brightness);
}