/*

Tested with HY-SRF05, HC-SR04
Assuming a room temp of 20 degrees centigrade

The circuit:
	* VVC connection of the sensor attached to +5V
	* GND connection of the sensor attached to ground
	* TRIG connection of the sensor attached to digital pin 12
        * ECHO connection of the sensor attached to digital pin 13
*/
 
// sonar 1
const int TRIG_PIN = 2;
const int ECHO_PIN = 3;

// sonar 2
//const int TRIG_PIN = 12;
//const int ECHO_PIN = 13;
 
void setup() {
  // initialize serial communication:
  Serial.begin(9600);
 
  pinMode(TRIG_PIN,OUTPUT);
  pinMode(ECHO_PIN,INPUT);
}
 
void loop()
{
    Serial.print(distance(12,13));
    Serial.print("cm  ------  ");
delay(100);    
    Serial.print(distance(2,3));
    Serial.print("cm--------");
  
  delay(100);
    Serial.print(distance(7,6));
    Serial.print("cm");
    Serial.println();
  delay(100);
}

float distance(int trig, int echo)
{ 
   
  long duration, distanceCm, distanceIn;
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  duration = pulseIn(echo,HIGH);
 
  // convert the time into a distance
  distanceCm = duration / 29.1 / 2 ;
  distanceIn = duration / 74 / 2;
 
 return distanceCm; 
}
