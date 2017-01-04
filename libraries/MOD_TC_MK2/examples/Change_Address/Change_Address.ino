#include <Wire.h>
#include <TC-MK2.h>

const unsigned char defaultAddress = 0x23;
const unsigned char newAddress = 0x30;
const int buttonPin = 2;

TC_MK2 tc(defaultAddress);
void setup()
{
  // Make sure that PROG jumper is set az input
  tc.pinMode(tc.GPIO_4, tc.IN);
  // Enable pullup on that pin
  tc.pullupMode(tc.PU_4, tc.ON);
}
void loop()
{
  if(digitalRead(buttonPin) == LOW){
    tc.setAddress(newAddress);
    while(digitalRead(buttonPin) == LOW);
  }
  
}
