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

void i2c_init();

/* START I2C Routine */
uint8_t i2c_transmit (uint8_t type);

uint8_t i2c_start (unsigned int dev_id, unsigned int dev_addr, uint8_t rw_type);

uint8_t i2c_stop (void);

uint8_t i2c_write (uint8_t data);

uint8_t i2c_read (uint8_t *data, uint8_t ack_type);

#endif /* I2C_H_ */
