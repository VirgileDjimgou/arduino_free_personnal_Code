#include <Wire.h>
#include <TC-MK2.h>

TC_MK2 tc(0x23);
void setup()
{
  Serial.begin(9600);
  tc.pinMode(  tc.GPIO_0 | 
               tc.GPIO_1 |
               tc.GPIO_2 |
               tc.GPIO_3 |
               tc.GPIO_4 |
               tc.GPIO_5 |
               tc.GPIO_6, tc.IN);
}
void loop()
{
  Serial.print("GPIO_0: ");
  Serial.println(tc.digitalRead(tc.GPIO_0), DEC);
  Serial.print("GPIO_1: ");
  Serial.println(tc.digitalRead(tc.GPIO_1), DEC);
  Serial.print("GPIO_2: ");
  Serial.println(tc.digitalRead(tc.GPIO_2), DEC);
  Serial.print("GPIO_3: ");
  Serial.println(tc.digitalRead(tc.GPIO_3), DEC);
  Serial.print("GPIO_4: ");
  Serial.println(tc.digitalRead(tc.GPIO_4), DEC);
  Serial.print("GPIO_5: ");
  Serial.println(tc.digitalRead(tc.GPIO_5), DEC);
  Serial.print("GPIO_6: ");
  Serial.println(tc.digitalRead(tc.GPIO_6), DEC);
  Serial.println("---------");
  delay(1000);
}


