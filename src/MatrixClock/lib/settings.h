/*
* settings.h
*
* Created: 24.01.2014 23:07:57
* Author: snakeye
*/
#ifndef SETTINGS_H_
#define SETTINGS_H_

typedef struct {
	
	uint8_t brightness;
	uint8_t brightness_auto;
	uint8_t brightness_min;
	uint8_t brightness_max;
} Settings;

extern Settings settings;

void load_settings();
void save_settings();
void apply_settings();

#endif /* SETTINGS_H_ */