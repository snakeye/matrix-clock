/*
 * buttons.h
 *
 * Created: 24.01.2014 23:09:40
 *  Author: snakeye
 */ 
#ifndef BUTTONS_H_
#define BUTTONS_H_

#define BUTTON_CLICK_TIME 10
#define BUTTON_HOLD_TIME 1000

void buttons_init();
void buttons_tick();

void on_button_click(uint8_t button);
void on_button_hold(uint8_t button);
void on_button_release(uint8_t button);

#endif /* BUTTONS_H_ */