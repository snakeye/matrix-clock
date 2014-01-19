/*
 * i2c.h
 *
 * Created: 04.02.2012 18:40:39
 *  Author: Andrey Ovcharov
 */
#ifndef I2C_H_
#define I2C_H_

#include <util/twi.h>

#define I2C_START 0
#define I2C_DATA 1
#define I2C_DATA_ACK 2
#define I2C_STOP 3
#define ACK 1
#define NACK 0

/* START I2C Routine */
unsigned char i2c_transmit (unsigned char type);

unsigned char i2c_start (unsigned int dev_id, unsigned int dev_addr,
			 unsigned char rw_type);
void i2c_stop (void);

unsigned char i2c_write (unsigned char data);

unsigned char i2c_read (unsigned char *data, unsigned char ack_type);

#endif /* I2C_H_ */
