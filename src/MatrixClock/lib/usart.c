/*
* usart.c
*
* Created: 24.01.2014 23:07:35
*  Author: snakeye
*/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>

#define RX_BUFFER_SIZE 32
#define TX_BUFFER_SIZE 8

char usart_command[64];
uint8_t usart_has_command;

uint8_t usart_rx_buffer[RX_BUFFER_SIZE];
uint8_t usart_rx_pos;

uint8_t usart_tx_buffer[TX_BUFFER_SIZE];
uint8_t usart_tx_pos;

ISR(USART_RXC_vect)
{
	uint8_t ch = UDR;
	
	// put received char into the buffer
	if(usart_rx_pos < RX_BUFFER_SIZE - 1)
	{
		usart_rx_buffer[usart_rx_pos] = ch;
	}
	
	// if EOL received
	if(ch == '\n')
	{
		usart_rx_buffer[usart_rx_pos + 1] = '\0';
		strcpy(usart_command, usart_rx_buffer);
		usart_rx_pos = 0;
		usart_has_command = 1;
	}
	else
	{
		if(usart_rx_pos < RX_BUFFER_SIZE - 1)
		{
			usart_rx_pos++;
		}
	}
}

ISR(USART_TXC_vect)
{
	if(usart_tx_pos >= TX_BUFFER_SIZE || usart_tx_buffer[usart_tx_pos] == '\0')
	{
		// stop transmitting
		UCSRB &= ~(1 << TXCIE);
	}
	else
	{
		UDR = usart_tx_buffer[usart_tx_pos];
		usart_tx_pos += 1;
	}
}

void usart_init(uint32_t baud)
{
	// calculate value for UBBR register
	uint16_t ubbr = F_CPU / (16 * baud) - 1;
	
	/* Set baud rate */
	UBRRH = (uint8_t)(ubbr >> 8);
	UBRRL = (uint8_t)ubbr;
	
	/* Enable receiver and transmitter */
	UCSRB = (1 << RXEN) | (1 << TXEN);
	
	/* Set frame format: 8data, 1stop bit */
	UCSRC = (1 << URSEL) | (0b011 << UCSZ0);
	
	//
	usart_rx_pos = 0;
	usart_tx_pos = 0;
	
	//
	usart_has_command = 0;
}

void usart_transmit_byte_sync(uint8_t data)
{
	/* Wait for empty transmit buffer */
	while (!(UCSRA & (1 << UDRE)))
	;
	/* Put data into buffer, sends the data */
	UDR = data;
}

void usart_transmit_sync(char* str)
{
	for(char* p = str; *p != '\0'; p++)
	{
		usart_transmit_byte_sync(*p);
	}
}

void usart_transmit_async(char* str)
{
	strcpy((char*)usart_tx_buffer, str);
	
	UDR = usart_tx_buffer[0];
	usart_tx_pos = 1;
		
	UCSRB |= (1 << TXCIE);
}

uint8_t usart_receive_byte_sync()
{
	/* Wait for data to be received */
	while (!(UCSRA & (1 << RXC)))
	;
	/* Get and return received data from buffer */
	return UDR;
}

void usart_receive_sync(char* buffer, uint8_t size)
{
	uint8_t i;
	for(i = 0; i < size; i++)
	{
		buffer[i] = usart_receive_byte_sync();
		if(buffer[i] == '\n')
		break;
	}
	buffer[i] = '\0';
}

void usart_receive_async()
{
	// Enable the USART Receive Complete interrupt (USART_RXC)
	UCSRB |= (1 << RXCIE);
}