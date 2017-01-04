/*
Using_GSM_and_GPS.pde - Example for using the GSM and GPS-Modul Rev.7
Included Functions
Version:     1.4.0
Date:        24.01.2013
Company:     antrax Datentechnik GmbH
Uses with:   Arduino Duemilanove (ATmega328) and
             Arduino UNO (ATmega328)
             
             
*/


#if defined(ARDUINO) && ARDUINO >= 100
  // Choose Arduino.h for IDE 1.0
  #include "Arduino.h"
#else
  // Choose WProgram.h if IDE is older than 1.0
  #include "WProgram.h"
#endif

#include <GSM_GPS_Shield.h>
#include <SPI.h>

int i=0;
                                                               // (power_pin, baudrate)
GPS gps(9600);                                                                   // (baudrate)

void setup()
{
  gps.initializeGPS();
  
}

void loop()
{
  gps.getGPS();
  //Serial.println(gps.gps_data_gprmc);
  Serial.println(gps.gps_data);
  
 
}


