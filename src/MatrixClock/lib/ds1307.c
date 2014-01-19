/*
 * ds1307.c
 *
 * Created: 04.02.2012 18:42:59
 *  Author: Andrey Ovcharov
 */

#include "ds1307.h"

#define DS1307_ID    0xD0	// I2C DS1307 Device Identifier
#define DS1307_ADDR  0x00	// I2C DS1307 Device Address

// DS1307 Register Address
// Second: ds1307_addr[0]
// Minute: ds1307_addr[1]
// Hour  : ds1307_addr[2]
// Day   : ds1307_addr[3]
// Date  : ds1307_addr[4]
// Month : ds1307_addr[5]
// Year  : ds1307_addr[6]
// control ds1307_addr[7]
unsigned char ds1307_addr[8] = { 0 };

unsigned char hour_mode;

unsigned char ampm_mode;

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

void ds1307_write ()
{
    unsigned char hour_format;

    // Make sure we enable the Oscillator control bit CH=0 on Register 0x00
    ds1307_addr[0] = ds1307_addr[0] & 0x7F;

    // Start the I2C Write Transmission
    i2c_start (DS1307_ID, DS1307_ADDR, TW_WRITE);

    // Start from Address 0x00
    i2c_write (0x00);

    // Write the data to the DS1307 address start at 0x00
    // DS1307 automatically will increase the Address.
    for (register int i = 0; i <= 7; i++)
    {
		if (i == 2)
		{
			hour_format = dec2bcd (ds1307_addr[i]);

			if (hour_mode)
			{
				hour_format |= (1 << 6);
			if (ampm_mode)
				hour_format |= (1 << 5);
			else
				hour_format &= ~(1 << 5);
			}
			else
			{
				hour_format &= ~(1 << 6);
			}

			i2c_write (hour_format);
		}
		else
		{
			i2c_write (dec2bcd (ds1307_addr[i]));
		}
    }

    // Stop I2C Transmission
    i2c_stop ();
}

void ds1307_read ()
{
    unsigned char data;

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
    ds1307_addr[0] = bcd2dec (data & 0x7F);

    // Read the Minute Register, Send Master Acknowledge
    i2c_read (&data, ACK);
    ds1307_addr[1] = bcd2dec (data);

    // Read the Hour Register, Send Master Acknowledge
    i2c_read (&data, ACK);
    if ((data & 0x40) == 0x40)
    {
		hour_mode = HOUR_12;
		ampm_mode = (data & 0x20) >> 5;	// ampm_mode: 0-AM, 1-PM
		ds1307_addr[2] = bcd2dec (data & 0x1F);
    }
    else
    {
		hour_mode = HOUR_24;
		ampm_mode = 0;
		ds1307_addr[2] = bcd2dec (data & 0x3F);
    }

    // Read the Day of Week Register, Send Master Acknowledge
    i2c_read (&data, ACK);
    ds1307_addr[3] = bcd2dec (data);

    // Read the Day of Month Register, Send Master Acknowledge
    i2c_read (&data, ACK);
    ds1307_addr[4] = bcd2dec (data);

    // Read the Month Register, Send Master Acknowledge
    i2c_read (&data, ACK);
    ds1307_addr[5] = bcd2dec (data);

    // Read the Year Register, Send Master No Acknowledge
    i2c_read (&data, NACK);
    ds1307_addr[6] = bcd2dec (data);

    // Read the control Register, Send Master No Acknowledge
    i2c_read (&data, NACK);
    ds1307_addr[7] = bcd2dec (data);

    // Stop I2C Transmission
    i2c_stop ();
}

// Convert Decimal to Binary Coded Decimal (BCD)
unsigned char dec2bcd (unsigned char num)
{
	return ((num / 10 * 16) + (num % 10));
}

// Convert Binary Coded Decimal (BCD) to Decimal
unsigned char bcd2dec (unsigned char num)
{
	return ((num / 16 * 10) + (num % 16));
}