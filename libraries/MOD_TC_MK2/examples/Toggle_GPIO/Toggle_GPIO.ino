#include <Wire.h>
#include <TC-MK2.h>

TC_MK2 tc(0x23);
void setup()
{
  tc.pinMode(tc.GPIO_0, tc.OUT);
}
void loop()
{
  tc.digitalWrite(tc.GPIO_0, tc.HI);
  delay(500);
  tc.digitalWrite(tc.GPIO_0, tc.LO);
  delay(500);
}


