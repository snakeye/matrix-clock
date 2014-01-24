/*
 * ds1307.h
 *
 * Created: 04.02.2012 18:42:43
 * Author: Andrey Ovcharov
 */
#ifndef DS1307_H_
#define DS1307_H_

#include "i2c.h"

#define HOUR_24 0
#define HOUR_12 1

// clock data
extern unsigned char ds1307_addr[8];

// reset clock chip
void ds1307_reset ();

// read clock data
void ds1307_read ();

// write clock data
void ds1307_write ();

// Convert Decimal to Binary Coded Decimal (BCD)
unsigned char dec2bcd (unsigned char num);

// Convert Binary Coded Decimal (BCD) to Decimal
unsigned char bcd2dec (unsigned char num);

#endif /* DS1307_H_ */
