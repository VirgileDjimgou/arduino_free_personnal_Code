/*
 * TC-MK2.h
 * 
 * Copyright 2013 OLIMEX LTD/Stefan Mavrodiev <support@olimex.com>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */

#ifndef TC_MK2_H
#define TC_MK2_H

#include <inttypes.h>

#define SET_ADDRESS 	0xF0
#define GET_ID		0x20
#define SET_TRIS	0x01
#define SET_LAT		0x02
#define GET_PORT	0x03
#define SET_PU		0x04

#define GET_TEMP	0x21

class TC_MK2
{
    private:
        static uint8_t address;	
	static uint8_t tris_status;
	static uint8_t lat_status;
	static uint8_t pu_status;
    public:
        TC_MK2();
        TC_MK2(uint8_t addr);
	void pinMode(uint8_t pin, uint8_t mode);
	void digitalWrite(uint8_t pin, uint8_t level);
	uint8_t digitalRead(uint8_t pin);
	void pullupMode(uint8_t pin, uint8_t state);
	uint16_t analogRead(uint8_t channel);
	uint32_t temperatureRead(void);
	
	void setAddress(uint8_t newAddress);
	uint8_t getID(void);	
	
	static uint8_t IN;
	static uint8_t OUT;
	static uint8_t LO;
	static uint8_t HI;
	static uint8_t ON;
	static uint8_t OFF;
	static uint8_t ANALOG_0;
	static uint8_t ANALOG_1;
	static uint8_t ANALOG_2;
	static uint8_t ANALOG_6;
	static uint8_t ANALOG_7;
	static uint8_t GPIO_0;
	static uint8_t GPIO_1;
	static uint8_t GPIO_2;
	static uint8_t GPIO_3;
	static uint8_t GPIO_4;
	static uint8_t GPIO_5;
	static uint8_t GPIO_6;
	static uint8_t PU_0;
	static uint8_t PU_1;
	static uint8_t PU_2;
	static uint8_t PU_3;
	static uint8_t PU_4;
	
};
#endif
