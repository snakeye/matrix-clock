/*
 * display.h
 *
 * Created: 19.01.2014 14:47:28
 *  Author: snakeye
 */ 
#ifndef DISPLAY_H_
#define DISPLAY_H_

#define DISPLAY_SEGMENTS 4
#define DISPLAY_QUEUE_LENGTH 16

// initialize display
void display_init();

// set display brightness
void display_set_brightness(uint8_t brightness);

// initialize display
void display_clear_canvas();

// set data for the whole column
void display_set_column(uint8_t col, uint8_t data);

// set single pixel
void display_set_pixel(uint8_t x, uint8_t y);

// clear single pixel
void display_clear_pixel(uint8_t x, uint8_t y);

//
void display_draw_sprite(int8_t x, int8_t y, uint8_t w, uint8_t h, const uint8_t* data);

//
void display_draw_string(int8_t x, int8_t y, const char* str);

// perform all canvas updates
void display_update();

// add command for segment
void display_push_cmd(uint8_t segment, uint8_t reg, uint8_t data);

// execute single command from command buffer
void display_execute();

// execute all commands from command buffer
void display_execute_all();

// test mode for display - all digits are on
void display_test_mode();

#endif /* DISPLAY_H_ */