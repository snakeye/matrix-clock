/*
* terminal.c
*
* Created: 25.01.2014 17:28:47
*  Author: snakeye
*/
#include <avr/io.h>
#include <string.h>

#include "brightness.h"
#include "ds1307.h"
#include "usart.h"
#include "string.h"

void process_terminal()
{
	char response[64];
	
	// hello
	if(strcmpn(usart_command, "Hello") == 0) {
		usart_transmit_async("Hello\r\n");
		return;
	}
	
	if(strcmpn(usart_command, "Time?") == 0) {
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
		stradd(response, "\r\n");
		usart_transmit_async(response);
		
		return;
	}

	if(strcmpn(usart_command, "Date?") == 0) {
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
		stradd(response, "\r\n");
		usart_transmit_async(response);
		
		return;
	}
	
	if(strcmpn(usart_command, "Weekday?") == 0) {
		response[0] = '0' + (ds1307_data.weekday);
		response[1] = '\0';
		
		//
		stradd(response, "\r\n");
		usart_transmit_async(response);
		
		return;
	}
	
	// asked brightness
	if(strcmpn(usart_command, "Light?") == 0){
		str_uint16dec(response, brightness, '\0');
		
		//
		stradd(response, "\r\n");
		usart_transmit_async(response);
		
		return;
	}
	
	if(strcmpn(usart_command, "Time=") == 0) {
		// check argument format
		for(uint8_t i = 5; i < 11; i++) {
			if(usart_command[i] < '0' || usart_command[i] > '9') {
				usart_transmit_async("Error\r\n");
				return;
			}
		}
		
		ds1307_data.hour = (usart_command[5] - '0') * 10 + (usart_command[6] - '0');
		ds1307_data.minute = (usart_command[7] - '0') * 10 + (usart_command[8] - '0');
		ds1307_data.second = (usart_command[9] - '0') * 10 + (usart_command[10] - '0');
		
		ds1307_write();
		
		return;
	}

	if(strcmpn(usart_command, "Date=") == 0) {
		// check argument format
		for(uint8_t i = 5; i < 11; i++) {
			if(usart_command[i] < '0' || usart_command[i] > '9') {
				usart_transmit_async("Error\r\n");
				return;
			}
		}
		
		ds1307_data.day = (usart_command[5] - '0') * 10 + (usart_command[6] - '0');
		ds1307_data.month = (usart_command[7] - '0') * 10 + (usart_command[8] - '0');
		ds1307_data.year = (usart_command[9] - '0') * 10 + (usart_command[10] - '0');
		
		ds1307_write();
		
		return;
	}

	if(strcmpn(usart_command, "Weekday=") == 0) {
		// check argument format
		if(usart_command[8] < '0' || usart_command[8] > '9') {
			usart_transmit_async("Error\r\n");
			return;
		}
		
		ds1307_data.weekday = (usart_command[8] - '0');
		
		ds1307_write();
		
		return;
	}
	
	if(strcmpn(usart_command, "Say ") == 0) {
		
		return;
	}
	
	usart_transmit_async("Error\r\n");
}
