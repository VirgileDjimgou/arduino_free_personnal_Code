/*
 * TC-MK2.cpp
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
 

#include "TC_MK2.h"
#include "Wire.h"



/*
 * Class variables
 */
 uint8_t TC_MK2::address = 0x23;
 uint8_t TC_MK2::tris_status = 0xFF;
 uint8_t TC_MK2::lat_status = 0x00;
 uint8_t TC_MK2::pu_status = 0x00;
 
 uint8_t TC_MK2::IN = 1;
 uint8_t TC_MK2::OUT= 0;
 uint8_t TC_MK2::ON = 1;
 uint8_t TC_MK2::OFF = 0;
 uint8_t TC_MK2::LO = 0;
 uint8_t TC_MK2::HI = 1;
 uint8_t TC_MK2::ANALOG_0 = 0x10;
 uint8_t TC_MK2::ANALOG_1 = 0x11;
 uint8_t TC_MK2::ANALOG_2 = 0x12;
 uint8_t TC_MK2::ANALOG_6 = 0x16;
 uint8_t TC_MK2::ANALOG_7 = 0x17;
 uint8_t TC_MK2::GPIO_0 = 0x01;
 uint8_t TC_MK2::GPIO_1 = 0x02;
 uint8_t TC_MK2::GPIO_2 = 0x04;
 uint8_t TC_MK2::GPIO_3 = 0x08;
 uint8_t TC_MK2::GPIO_4 = 0x10;
 uint8_t TC_MK2::GPIO_5 = 0x20;
 uint8_t TC_MK2::GPIO_6 = 0x40;
 uint8_t TC_MK2::PU_0 = 0x01;
 uint8_t TC_MK2::PU_1 = 0x02;
 uint8_t TC_MK2::PU_2 = 0x04;
 uint8_t TC_MK2::PU_3 = 0x08;
 uint8_t TC_MK2::PU_4 = 0x10;

/*
 * Constructors
 */
TC_MK2::TC_MK2(){
}

TC_MK2::TC_MK2(uint8_t addr){
    address = addr;
}

void TC_MK2::setAddress(uint8_t newAddress){
    Wire.begin();
    Wire.beginTransmission(address);
    Wire.write(SET_ADDRESS);
    Wire.write(newAddress);
    Wire.endTransmission();
}
uint8_t TC_MK2::getID(void){
    unsigned char data;  
    Wire.begin();
    Wire.beginTransmission(address);
    Wire.write(GET_ID);
    Wire.endTransmission();
    Wire.requestFrom((int)address, 1);
    data = Wire.read();
    
    return data;    
}

void TC_MK2::pinMode(uint8_t pin, uint8_t mode){
  if(mode == IN){
    tris_status |= pin;
  } else {
    tris_status &= ~pin;
  }
  Wire.begin();
  Wire.beginTransmission(address);
  Wire.write(SET_TRIS);
  Wire.write(tris_status);
  Wire.endTransmission();
}

void TC_MK2::digitalWrite(uint8_t pin, uint8_t level){
  if(level == HI)
    lat_status |= pin;
  else
    lat_status &= ~pin;
  
  Wire.begin();
  Wire.beginTransmission(address);
  Wire.write(SET_LAT);
  Wire.write(lat_status);
  Wire.endTransmission();
}

uint8_t TC_MK2::digitalRead(uint8_t pin){
  uint8_t data;
  
  Wire.begin();
  Wire.beginTransmission(address);
  Wire.write(GET_PORT);
  Wire.endTransmission();
  Wire.requestFrom((int)address, 1);
  while(Wire.available() > 0)
    data = Wire.read();
  
  return (data & pin) ? 1 : 0;
}

uint16_t TC_MK2::analogRead(uint8_t channel){
  uint16_t adc;
  uint8_t data[2];
  uint8_t i = 0;
  Wire.begin();
  Wire.beginTransmission(address);
  Wire.write(channel);
  Wire.endTransmission();
  Wire.requestFrom((int)address, 2);
  while(Wire.available() > 0){
    data[i++] = Wire.read();
  }
  adc = data[1];
  adc <<= 8;
  adc |= data[0];
  
  return adc; 
  
}

uint32_t TC_MK2::temperatureRead(){
  uint32_t temp;
  uint8_t data[4];
  uint8_t i = 0;
  
  Wire.begin();
  Wire.beginTransmission(address);
  Wire.write(GET_TEMP);
  Wire.endTransmission();
  Wire.requestFrom((int)address, 4);
  while(Wire.available() > 0)
    data[i++] = Wire.read();
  
  temp = data[3];
  temp <<= 8;
  temp |= data[2];
  temp <<= 8;
  temp |= data[1];
  temp <<= 8;
  temp |= data[0];
  
  return temp;
}

void TC_MK2::pullupMode(uint8_t pin, uint8_t state){
  if(state == ON)
    pu_status |= pin;
  else
    pu_status &= ~pin;
  
  Wire.begin();
  Wire.beginTransmission(address);
  Wire.write(SET_PU);
  Wire.write(pu_status);
  Wire.endTransmission();
}