#include <Wire.h>
#include <TC-MK2.h>

const unsigned char defaultAddress = 0x23;
const int buttonPin = 2;
TC_MK2 tc(defaultAddress);
void setup()
{
  Serial.begin(9600);
  pinMode(buttonPin, INPUT);
}
void loop()
{
  if(digitalRead(buttonPin) == LOW){
    unsigned char c = tc.getID();
    Serial.print("DEVICE ID: 0x");
    Serial.println(c, HEX);
    while(digitalRead(buttonPin) == LOW);
  }
  
}

