/*
* i2c.c
*
* Created: 04.02.2012 18:40:28
*  Author: Andrey Ovcharov
*/
#include "i2c.h"

#define MAX_TRIES 10

void i2c_init()
{
	// Select TWI Prescaler of 1
	TWSR = 0x00;
	// TWI SCL frequency = 100 khz
	TWBR = 32;
}

/**
*
*/
uint8_t i2c_transmit (uint8_t type)
{
	switch (type)
	{
		case I2C_START:
		// Send Start Condition
		TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
		break;
		case I2C_DATA:
		// Send Data with No-Acknowledge
		TWCR = (1 << TWINT) | (1 << TWEN);
		break;
		case I2C_DATA_ACK:
		// Send Data with Acknowledge
		TWCR = (1 << TWEA) | (1 << TWINT) | (1 << TWEN);
		break;
		case I2C_STOP:
		// Send Stop Condition
		TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
		return 0;
	}

	// Wait for TWINT flag set on Register TWCR
	while (!(TWCR & (1 << TWINT)))
	{
	}

	// Return TWI Status Register, mask the prescaller bits (TWPS1,TWPS0)
	return (TWSR & 0xF8);
}

uint8_t i2c_start (unsigned int dev_id, unsigned int dev_addr, uint8_t rw_type)
{
	uint8_t twi_status;

	for (register int n = 0; n < MAX_TRIES; n++)
	{
		// Transmit Start Condition
		twi_status = i2c_transmit (I2C_START);

		// Check the TWI Status
		if (twi_status == TW_MT_ARB_LOST)
		continue;

		if ((twi_status != TW_START) && (twi_status != TW_REP_START))
		break;

		// Send slave address (SLA_W)
		TWDR = (dev_id & 0xF0) | (dev_addr & 0x07) | rw_type;

		// Transmit I2C Data
		twi_status = i2c_transmit (I2C_DATA);

		// Check the TWSR status
		if ((twi_status == TW_MT_SLA_NACK) || (twi_status == TW_MT_ARB_LOST))
		continue;

		if (twi_status != TW_MT_SLA_ACK)
		break;

		return 0;
	}

	return -1;
}

/**
*
*/
uint8_t i2c_stop (void)
{
	return i2c_transmit (I2C_STOP);
}

/**
*
*/
uint8_t i2c_write (uint8_t data)
{
	uint8_t twi_status;

	// Send the Data to I2C Bus
	TWDR = data;

	// Transmit I2C Data
	twi_status = i2c_transmit (I2C_DATA);

	// Check the TWSR status
	if (twi_status != TW_MT_DATA_ACK)
	return -1;

	return 0;
}

uint8_t i2c_read (uint8_t *data, uint8_t ack_type)
{
	uint8_t twi_status;

	if (ack_type)
	{
		// Read I2C Data and Send Acknowledge
		twi_status = i2c_transmit (I2C_DATA_ACK);

		if (twi_status != TW_MR_DATA_ACK)
		return -1;
	}
	else
	{
		// Read I2C Data and Send No Acknowledge
		twi_status = i2c_transmit (I2C_DATA);

		if (twi_status != TW_MR_DATA_NACK)
		return -1;
	}

	// Get the Data
	*data = TWDR;

	return 0;
}
