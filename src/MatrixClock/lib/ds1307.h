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

// DS1307 Register Address
// Second: ds1307_addr[0]
// Minute: ds1307_addr[1]
// Hour  : ds1307_addr[2]
// Day   : ds1307_addr[3]
// Date  : ds1307_addr[4]
// Month : ds1307_addr[5]
// Year  : ds1307_addr[6]
// control ds1307_addr[7]
typedef struct {
	uint8_t second;
	uint8_t minute;
	uint8_t hour;
	uint8_t weekday;
	uint8_t day;
	uint8_t month;
	uint8_t year;
	uint8_t control;
} DS1307Data;

extern DS1307Data ds1307_data;

// reset clock chip
void ds1307_reset ();

// read clock data
void ds1307_read ();

// write clock data
void ds1307_write ();

// Convert Decimal to Binary Coded Decimal (BCD)
uint8_t dec2bcd (uint8_t num);

// Convert Binary Coded Decimal (BCD) to Decimal
uint8_t bcd2dec (uint8_t num);

void ds1307_tick();

// time changed event
void on_time_changed();

// date changed event
void on_date_changed();

#endif /* DS1307_H_ */
