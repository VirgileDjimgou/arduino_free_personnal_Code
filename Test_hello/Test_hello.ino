
#include <Servo.h> 
int ledPin = 9;

String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
Servo frontServo;  // create servo object to control a servo 

int steeringAngle = 100;
int motorSpeed = 100;

void setup() {
  // initialize serial:
  frontServo.attach(9);  // attaches the servo on pin 9 to the servo object 
  Serial.begin(9600);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
}

void loop() {
  // print the string when a newline arrives:
  if (stringComplete) {

    stringComplete = false;
    //analogWrite(ledPin, inputString.toInt());
     // steeringServo.write(inputString.toInt()); 
     setSpeedPercentage(inputString.toInt());
   Serial.println(inputString);
   delay (3000);
   inputString = "";
  }
}


/* sets the speed in percent */
void setSpeedPercentage(int value){
  /* Speed servo options:
     0 = 100% backwards
     90 = stop (none)
     180 = 100% forward
  */
  Serial.println("speedPercent :"+value);
  if(value==0){
    frontServo.write(90);
    // rearServo.write(90);
  }if(value>0&&value<101){
    /* drive forward */
    int driveAngle = (int)(90+(70*(float)((float)value/100)));
    frontServo.write(driveAngle);
    // rearServo.write(driveAngle);
  }if(value>100&&value<201){
    /* drive backward */
    int driveAngle = (int)(90-(70*(float)(((float)value-100)/100)));
    frontServo.write(driveAngle);
    // rearServo.write(driveAngle);
  }
  
  /* update the local variable */
  motorSpeed = value;
}



void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}
