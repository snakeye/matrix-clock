/*
 * string.c
 *
 * Created: 24.01.2014 23:06:48
 *  Author: snakeye
 */ 
#include <avr/io.h>

void str_uint16dec(char* str, uint16_t value)
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