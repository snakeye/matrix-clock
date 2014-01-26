/*
 * string.h
 *
 * Created: 24.01.2014 23:05:50
 *  Author: snakeye
 */ 
#ifndef STRING_H_
#define STRING_H_

uint8_t strcmpn(const char* buffer, const char* cmd);

//
void stradd(char* buffer, const char* str);

//
void str_uint8hex(char* str, uint8_t value);

//
void str_uint8dec(char* str, uint8_t value);

//
void str_int8dec(char* str, uint16_t value);

//
void str_uint16dec(char* str, uint16_t value, char fill);

//
void str_int16dec(char* str, uint16_t value);

//
void str_uint16hex(char* str, uint16_t value);


#endif /* STRING_H_ */