int ledPin = 9;


void setup()  {
Serial.begin (9600);
}
void loop()  {
  
  [code]
  if(Serial.available()>0){
    /* read command from serial */
    byte buffer[3];
    Serial.readBytes((char*)buffer,3);
    if(sizeof(buffer)==3){
      /* byte 222 is the terminator */
      if((int)buffer[0]==222){
        setSteeringPercentage((int)buffer[1]);
        setSpeedPercentage((int)buffer[2]);
      }if((int)buffer[1]==222){
        setSteeringPercentage((int)buffer[2]);
        setSpeedPercentage((int)buffer[0]);
      }if((int)buffer[2]==222){
        setSteeringPercentage((int)buffer[0]);
        setSpeedPercentage((int)buffer[1]);
      }
    }
    
  }

[/code]
   for (int i = 800; i < 900; i++){
   analogWrite(ledPin, i);
   Serial.println(i);
   delay (3000);
 }
}
