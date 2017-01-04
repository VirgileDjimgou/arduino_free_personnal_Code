#include <Wire.h>
#include <TC-MK2.h>

TC_MK2 tc(0x23);
void setup()
{
  Serial.begin(9600);
}
void loop()
{
  
  uint32_t c = tc.temperatureRead();
  // Check for fault
  if(c & 0x00010000){
    // Fault detected
    Serial.print("FAULT: ");
    if (c & 0x00000001)
      Serial.println("Open Circuit");
     else if (c & 0x00000002)
       Serial.println("Short to GND");
     else if (c & 0x00000004)
       Serial.println("Short to VCC");
  }else{
    float in_f;
    float ex_f;
    unsigned char in_s = !!(c & 0x00008000);
    unsigned char ex_s = !!(c & 0x80000000);
    /* Convert external temperature */
    uint16_t ext = (c >> 16 );
    ext &= (0x7FFF);
    ext >>= 2;
    ex_f = (ext >> 2) + (ext & 0x03) * 0.25;
    if(ex_s)
      ex_f = -2048 + ex_f;
      
    /* Convert internal temperature */
    uint16_t in = c & 0x0000FFFF;
    in >>= 4;
    in_f = (in >> 4) + (in & 0x0F) * 0.0625;
    if(in_s)
     in_s = -128 + in_s; 
     
    Serial.print("External temperature: ");
    Serial.println(ex_f, 2);
    Serial.print("Internal temperature: ");
    Serial.println(in_f, 4);
  }
  Serial.println("---------");
  delay(1000);
}


