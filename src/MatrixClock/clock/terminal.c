/*
* terminal.c
*
* Created: 25.01.2014 17:28:47
*  Author: snakeye
*/
#include <avr/io.h>
#include <string.h>

#include "../lib/ds1307.h"
#include "../lib/usart.h"
#include "../lib/string.h"

#include "display_mode.h"
#include "brightness.h"
#include "marquee.h"

/**
 * 
 */
void terminal_handshake()
{
	usart_transmit_async("Matrix Clock\r\n");
}

/**
 * 
 */
void terminal_bye()
{
	usart_transmit_async("Bye!\r\n");
}

/**
 * 
 */
void terminal_get_time()
{
	char response[16];
	
	response[0] = '0' + (ds1307_data.hour / 10);
	response[1] = '0' + (ds1307_data.hour % 10);
	response[2] = ':';
	response[3] = '0' + (ds1307_data.minute / 10);
	response[4] = '0' + (ds1307_data.minute % 10);
	response[5] = ':';
	response[6] = '0' + (ds1307_data.second / 10);
	response[7] = '0' + (ds1307_data.second % 10);
	response[8] = '\0';
	
	//
	strcat(response, "\r\n");
	usart_transmit_async(response);	
}

/**
 * 
 */
void terminal_get_date()
{
	char response[16];
	
	response[0] = '0' + (ds1307_data.day / 10);
	response[1] = '0' + (ds1307_data.day % 10);
	response[2] = '/';
	response[3] = '0' + (ds1307_data.month / 10);
	response[4] = '0' + (ds1307_data.month % 10);
	response[5] = '/';
	response[6] = '0' + (ds1307_data.year / 10);
	response[7] = '0' + (ds1307_data.year % 10);
	response[8] = '\0';
	
	//
	strcat(response, "\r\n");
	usart_transmit_async(response);	
}

/**
 * 
 */
void terminal_get_weekday()
{
	char response[16];
	
	response[0] = '0' + (ds1307_data.weekday);
	response[1] = '\0';
	
	//
	strcat(response, "\r\n");
	usart_transmit_async(response);
}

/**
 * 
 */
void terminal_get_light()
{
	char response[16];
	
	str_uint16dec(response, brightness, '\0');
	
	//
	strcat(response, "\r\n");
	usart_transmit_async(response);
}

/**
 * 
 */
void terminal_set_time()
{
	// check argument format
	for(uint8_t i = 5; i < 11; i++) {
		if(usart_command[i] < '0' || usart_command[i] > '9') {
			usart_transmit_async("Wrong time format\r\n");
			return;
		}
	}
	
	ds1307_data.hour = (usart_command[5] - '0') * 10 + (usart_command[6] - '0');
	ds1307_data.minute = (usart_command[7] - '0') * 10 + (usart_command[8] - '0');
	ds1307_data.second = (usart_command[9] - '0') * 10 + (usart_command[10] - '0');
	
	ds1307_write();

	//
	usart_transmit_async("OK\r\n");
}

/**
 * 
 */
void terminal_set_date()
{
	// check argument format
	for(uint8_t i = 5; i < 11; i++) {
		if(usart_command[i] < '0' || usart_command[i] > '9') {
			usart_transmit_async("Wrong date format\r\n");
			return;
		}
	}
	
	ds1307_data.day = (usart_command[5] - '0') * 10 + (usart_command[6] - '0');
	ds1307_data.month = (usart_command[7] - '0') * 10 + (usart_command[8] - '0');
	ds1307_data.year = (usart_command[9] - '0') * 10 + (usart_command[10] - '0');
	
	ds1307_write();
	
	//
	usart_transmit_async("OK\r\n");
}

/**
 * 
 */
void terminal_set_weekday()
{
	// check argument format
	if(usart_command[8] <= '0' || usart_command[8] >= '8') {
		usart_transmit_async("Wrong weekday format\r\n");
		return;
	}
	
	ds1307_data.weekday = (usart_command[8] - '0');
	
	ds1307_write();

	//
	usart_transmit_async("OK\r\n");
	
}

/**
 * 
 */
void terminal_error()
{
	char response[64];
	
	strcpy(response, "Wrong command: ");
	strcat(response, usart_command);
	strcat(response, "\r\n");
	usart_transmit_async(response);
}

/**
 * 
 */
void process_terminal()
{
	if(strcmp_n(usart_command, "hello", 5) == 0) {
		terminal_handshake();
		return;
	}

	if(strcmp_n(usart_command, "bye", 3) == 0) {
		terminal_bye();
		return;
	}
	
	if(strcmp_n(usart_command, "time?", 5) == 0) {
		terminal_get_time();
		return;
	}

	if(strcmp_n(usart_command, "date?", 5) == 0) {
		terminal_get_date();
		return;
	}
	
	if(strcmp_n(usart_command, "weekday?", 8) == 0) {
		terminal_get_weekday();
		return;
	}
	
	// asked brightness
	if(strcmp_n(usart_command, "light?", 6) == 0){
		terminal_get_light();
		return;
	}
	
	if(strcmp_n(usart_command, "time=", 5) == 0) {
		terminal_set_time();
		return;
	}

	if(strcmp_n(usart_command, "date=", 5) == 0) {
		terminal_set_date();
		return;
	}

	if(strcmp_n(usart_command, "weekday=", 8) == 0) {
		terminal_set_weekday();
		return;
	}
	
	terminal_error();
}
