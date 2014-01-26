/*
* ds1307.c
*
* Created: 04.02.2012 18:42:59
*  Author: Andrey Ovcharov
*/

#include "ds1307.h"

#define DS1307_ID    0xD0	// I2C DS1307 Device Identifier
#define DS1307_ADDR  0x00	// I2C DS1307 Device Address

#define DS1307_SECOND 0
#define DS1307_MINUTE 1
#define DS1307_HOUR 2
#define DS1307_WEEKDAY 3
#define DS1307_DAY 4
#define DS1307_MONTH 5
#define DS1307_YEAR 6
#define DS1307_CONTROL 7

DS1307Data ds1307_data = {0, 0, 0, 1, 1, 1, 0, 0};
uint8_t hour_mode;
uint8_t ampm_mode;

/**
*
*/
void ds1307_reset ()
{
	// Start the I2C Write Transmission
	i2c_start (DS1307_ID, DS1307_ADDR, TW_WRITE);

	// Start from Address 0x00
	i2c_write (0x00);

	i2c_write (0x00);

	// Stop I2C Transmission
	i2c_stop ();
}

/**
*
*/
void ds1307_tick()
{
	static uint8_t tick = 0;
	static uint8_t hour = 0;
	static uint8_t minute = 0;
	static uint8_t day = 0;
	static uint8_t month = 0;
	
	tick++;
	if(tick >= 100) {
		tick = 0;

		ds1307_read();
		
		if(hour != ds1307_data.hour || minute != ds1307_data.minute) {
			on_time_changed();
			hour = ds1307_data.hour;
			minute = ds1307_data.minute;
		}	
	
		if(month != ds1307_data.month || day != ds1307_data.day) {
			on_date_changed();
			month = ds1307_data.month;
			day = ds1307_data.day;
		}
	}
}

/**
*
*/
void ds1307_write ()
{
	uint8_t hour_format;

	// Make sure we enable the Oscillator control bit CH=0 on Register 0x00
	ds1307_data.second &= 0x7F;

	// Start the I2C Write Transmission
	i2c_start (DS1307_ID, DS1307_ADDR, TW_WRITE);

	// Start from Address 0x00
	i2c_write (0x00);

	// Write the data to the DS1307 address start at 0x00
	// DS1307 automatically will increase the Address.
	i2c_write (dec2bcd(ds1307_data.second));
	i2c_write (dec2bcd(ds1307_data.minute));
	
	hour_format = dec2bcd(ds1307_data.hour);
	if (hour_mode)
	{
		hour_format |= (1 << 6);
		if (ampm_mode) hour_format |= (1 << 5);
		else hour_format &= ~(1 << 5);
	}
	else
	{
		hour_format &= ~(1 << 6);
	}

	i2c_write (hour_format);

	i2c_write (dec2bcd(ds1307_data.weekday));
	i2c_write (dec2bcd(ds1307_data.day));
	i2c_write (dec2bcd(ds1307_data.month));
	i2c_write (dec2bcd(ds1307_data.year));
	i2c_write (dec2bcd(ds1307_data.control));

	// Stop I2C Transmission
	i2c_stop ();
}

/**
*
*/
void ds1307_read ()
{
	uint8_t data;

	// First we initial the pointer register to address 0x00
	// Start the I2C Write Transmission
	i2c_start (DS1307_ID, DS1307_ADDR, TW_WRITE);

	// Start from Address 0x00
	i2c_write (0x00);

	// Stop I2C Transmission
	i2c_stop ();

	// Start the I2C Read Transmission
	i2c_start (DS1307_ID, DS1307_ADDR, TW_READ);

	// Read the Second Register, Send Master Acknowledge
	i2c_read (&data, ACK);
	ds1307_data.second = bcd2dec (data & 0x7F);

	// Read the Minute Register, Send Master Acknowledge
	i2c_read (&data, ACK);
	ds1307_data.minute = bcd2dec (data);

	// Read the Hour Register, Send Master Acknowledge
	i2c_read (&data, ACK);
	if ((data & 0x40) == 0x40)
	{
		hour_mode = HOUR_12;
		ampm_mode = (data & 0x20) >> 5;	// ampm_mode: 0-AM, 1-PM
		ds1307_data.hour = bcd2dec (data & 0x1F);
	}
	else
	{
		hour_mode = HOUR_24;
		ampm_mode = 0;
		ds1307_data.hour = bcd2dec (data & 0x3F);
	}

	// Read the Day of Week Register, Send Master Acknowledge
	i2c_read (&data, ACK);
	ds1307_data.weekday = bcd2dec (data);

	// Read the Day of Month Register, Send Master Acknowledge
	i2c_read (&data, ACK);
	ds1307_data.day = bcd2dec (data);

	// Read the Month Register, Send Master Acknowledge
	i2c_read (&data, ACK);
	ds1307_data.month = bcd2dec (data);

	// Read the Year Register, Send Master No Acknowledge
	i2c_read (&data, NACK);
	ds1307_data.year = bcd2dec (data);

	// Read the control Register, Send Master No Acknowledge
	i2c_read (&data, NACK);
	ds1307_data.control = bcd2dec (data);

	// Stop I2C Transmission
	i2c_stop ();
}

// Convert Decimal to Binary Coded Decimal (BCD)
uint8_t dec2bcd (uint8_t num)
{
	return ((num / 10 * 16) + (num % 10));
}

// Convert Binary Coded Decimal (BCD) to Decimal
uint8_t bcd2dec (uint8_t num)
{
	return ((num / 16 * 10) + (num % 16));
}