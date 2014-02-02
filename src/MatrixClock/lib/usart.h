/*
 * usart.h
 *
 * Created: 24.01.2014 23:07:25
 *  Author: snakeye
 */ 
#ifndef USART_H_
#define USART_H_

//
extern char usart_command[];
extern uint8_t usart_has_command; 

//
void usart_init(uint32_t baud);

//
void usart_transmit_byte_sync(uint8_t byte);

//
void usart_transmit_sync(const char* str);

//
void usart_transmit_async(const char* str);

//
uint8_t usart_receive_byte_sync();

// 
void usart_receive_sync(char* buffer, uint8_t size);

//
void usart_receive_async();


#endif /* USART_H_ */