/*
* string.c
*
* Created: 24.01.2014 23:06:48
*  Author: snakeye
*/
#include <avr/io.h>
#include <string.h>

uint8_t strcmpn(const char* buffer, const char* cmd)
{
	uint8_t i;
	for(i = 0; cmd[i] != '\0'; i++) {
		if(buffer[i] != cmd[i]) {
			return 1;
		}
	}
	
	return 0;
}

void stradd(char* buffer, const char* str)
{
	uint8_t i;
	for(i = 0; buffer[i] != '\0'; i++) {
		
	}
	strcpy(&buffer[i], str);
}

/**
 * 
 */
void str_uint16dec(char* str, uint16_t value, char fill)
{
	uint8_t skip = 1;
	uint8_t pos = 0;
	
	uint16_t divisor = 10000;
	while(divisor > 0)
	{
		uint8_t digit = (value % (divisor * 10)) / divisor;
		
		if(digit == 0)
		{
			if(!skip || divisor == 1)
			{
				str[pos++] = '0';
			}
		}
		else
		{
			str[pos++] = '0' + digit;
			skip = 0;
		}
		
		divisor = divisor / 10;
	}
	
	str[pos] = '\0';
}