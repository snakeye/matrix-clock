/*
* usart.c
*
* Created: 24.01.2014 23:07:35
*  Author: snakeye
*/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include <ctype.h>

#define COMMAND_SIZE 64
#define RX_BUFFER_SIZE 64
#define TX_BUFFER_SIZE 64

uint8_t usart_rx_buffer[RX_BUFFER_SIZE];
uint8_t usart_rx_pos;

uint8_t usart_tx_buffer[TX_BUFFER_SIZE];
uint8_t usart_tx_pos;

char usart_command[COMMAND_SIZE];
uint8_t usart_has_command;

#define USART_RX_FULL (usart_rx_pos >= RX_BUFFER_SIZE - 1)

/**
* Append char to USART RX buffer
*/
inline void usart_buffer_append(uint8_t ch)
{
	// commands are transformed to lower case
	usart_rx_buffer[usart_rx_pos] = tolower(ch);
	usart_rx_buffer[usart_rx_pos + 1] = '\0';
	usart_rx_pos++;
}

/**
* Check if command terminal char
*/
inline uint8_t isterminal(uint8_t ch)
{
	if(ch == '\0' || ch == '\n' || ch == '\r') {
		return 1;
	}
	
	return 0;
}

/**
* USART input interrupt
*/
ISR(USART_RXC_vect)
{
	uint8_t ch = UDR;

	// if not space
	if(!USART_RX_FULL && !isspace(ch)) {
		usart_buffer_append(ch);
	}
	
	// finish command
	if(isterminal(ch) || USART_RX_FULL) {
		
		// if we have something in the buffer
		if(usart_rx_pos != 0) {
			// copy buffer to current command
			strcpy(&usart_command[0], (char*)&usart_rx_buffer[0]);
			usart_has_command = 1;
		}
		
		// reset buffer
		usart_rx_pos = 0;
	}
}

/**
* USART output success interrupt
*/
ISR(USART_TXC_vect)
{
}

/**
* Data register is empty
*/
ISR(USART_UDRE_vect)
{
	// get next char to transmit
	uint8_t b = usart_tx_buffer[usart_tx_pos];
	
	if(b != '\0') {
		// transmit it
		UDR = b;
		usart_tx_pos += 1;
	}
	else {
		// stop transmitting
		UCSRB &= ~(1 << UDRIE);
	}
}

/**
*
*/
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
	
	// reset buffer positions
	usart_rx_pos = 0;
	usart_tx_pos = 0;
	
	// reset command flag
	usart_has_command = 0;
}

/**
*
*/
void usart_transmit_byte_sync(uint8_t data)
{
	/* Wait for empty transmit buffer */
	while (!(UCSRA & (1 << UDRE)))
	;
	/* Put data into buffer, sends the data */
	UDR = data;
}

/**
*
*/
void usart_transmit_sync(const char* str)
{
	for(char* p = (char*)str; *p != '\0'; p++)
	{
		usart_transmit_byte_sync(*p);
	}
}

/**
*
*/
void usart_transmit_async(const char* str)
{
	strcpy((char*)&usart_tx_buffer[0], str);
	usart_tx_pos = 0;
	
	//UDR = usart_tx_buffer[0];
	//usart_tx_pos = 1;
	
	// start transmitting
	//UCSRB |= (1 << TXCIE);
	UCSRB |= (1 << UDRIE);
}

/**
*
*/
uint8_t usart_receive_byte_sync()
{
	/* Wait for data to be received */
	while (!(UCSRA & (1 << RXC)))
	;
	/* Get and return received data from buffer */
	return UDR;
}

/**
*
*/
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

/**
*
*/
void usart_receive_async()
{
	// Enable the USART Receive Complete interrupt (USART_RXC)
	UCSRB |= (1 << RXCIE);
}
