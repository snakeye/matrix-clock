/*
* settings.c
*
* Created: 24.01.2014 23:08:08
*  Author: snakeye
*/
#include <avr/io.h>

#include "settings.h"
#include "display.h"

Settings settings;

void default_settings()
{
	settings.brightness = 0x08;
	settings.brightness_min = 0;
	settings.brightness_max = 0x0f;
	settings.brightness_auto = 1;
}

void load_settings()
{
	default_settings();
}

void save_settings()
{
	
}

void apply_settings()
{
	display_set_brightness(settings.brightness);
}