/*
 * max7219.h
 *
 * Created: 15.11.2013 21:43:05
 *  Author: snakeye
 */ 
#ifndef MAX7219_H_
#define MAX7219_H_

#define MAX7219_PORT PORTC
#define MAX7219_DIN PC5
#define MAX7219_CLK PC7
#define MAX7219_LOAD PC6

#define MAX7219_REG_NOOP 0x00
#define MAX7219_REG_DECODE 0x09
#define MAX7219_REG_BRIGHTNESS 0x0A
#define MAX7219_REG_SCAN 0x0B
#define MAX7219_REG_SHUTDOWN 0x0C
#define MAX7219_REG_TEST 0x0F

// start send operation
void max7219_start();
// send single byte
void max7219_send(uint8_t data);
// end send operation
void max7219_latch();

// write data to register
void max7219_write(uint8_t reg, uint8_t data);

#endif /* MAX7219_H_ */