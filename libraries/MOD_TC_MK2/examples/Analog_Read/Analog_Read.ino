#include <Wire.h>
#include <TC-MK2.h>

TC_MK2 tc(0x23);
void setup()
{
  Serial.begin(9600);
}
void loop()
{
  /* READ ANALOG 0 */
  unsigned int an0 = tc.analogRead(tc.ANALOG_0);
  Serial.print("ANALOG_0: ");
  Serial.print((3.3/1023) * an0);
  Serial.println("V");
  /* READ ANALOG 1 */
  unsigned int an1 = tc.analogRead(tc.ANALOG_1);
  Serial.print("ANALOG_1: ");
  Serial.print((3.3/1023) * an1);
  Serial.println("V");
  /* READ ANALOG 2 */
  unsigned int an2 = tc.analogRead(tc.ANALOG_2);
  Serial.print("ANALOG_2: ");
  Serial.print((3.3/1023) * an2);
  Serial.println("V");
  /* READ ANALOG 6 */
  unsigned int an6 = tc.analogRead(tc.ANALOG_6);
  Serial.print("ANALOG_6: ");
  Serial.print((3.3/1023) * an6);
  Serial.println("V");
  /* READ ANALOG 7 */
  unsigned int an7 = tc.analogRead(tc.ANALOG_7);
  Serial.print("ANALOG_7: ");
  Serial.print((3.3/1023) * an7);
  Serial.println("V");
  Serial.println("---------");
  delay(1000);
}


