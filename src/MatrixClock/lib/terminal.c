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
	if(strcmp(usart_command, "Hello\r\n") == 0) {
		usart_transmit_async("Hello\r\n");
		return;
	}
	
	if(strcmp(usart_command, "Time?\r\n") == 0) {
		char buffer[10];
		str_uint16dec(response, ds1307_addr[2]);
		
		stradd(response, ":");
		
		str_uint16dec(buffer, ds1307_addr[1]);
		stradd(response, buffer);
		
		stradd(response, ":");
		
		str_uint16dec(buffer, ds1307_addr[0]);
		stradd(response, buffer);
		
		//
		stradd(response, "\r\n");
		usart_transmit_async(response);
		
		return;
	}
	
	// asked brightness
	if(strcmp(usart_command, "Light?\r\n") == 0){
		str_uint16dec(response, brightness);
		
		//
		stradd(response, "\r\n");
		usart_transmit_async(response);
		
		return;
	}
	
	usart_transmit_async("Error\r\n");
}
