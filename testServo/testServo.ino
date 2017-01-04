#include <Servo.h>
Servo steering;  // create servo object called "steering" to control the steering servo
Servo throttle;
int pos = 0;    // variable to store the steering position
void setup()
{
  steering.attach(9);  // tells the arduino that the object "steering" should be associated with digital pin 9
  throttle.attach(6);  // and the throttle to pin 8
  Serial.begin(9600);
}
void loop()
{
  delay(4000);
  steering.write(90); //Center the steering
   //arm the Electronic Speed Controller (ESC)
  delay(1000); //gonna waggle the front wheels a bit to test the steering
  int max = 5;
  int i = 0;
  for(i = 0; i < max;i++){
    for(pos = 58; pos < 132; pos += 10)  // goes from 58 degrees (full right) to 132 degrees (full left)
    {                                  // in steps of 10 degree (fast)
      steering.write(pos);              // writes the position to the servo
      Serial.println("Steering :"+pos);
      delay(15);                       // waits 30ms for the servo to reach the position
    }
    for(pos = 132; pos>=58; pos-=10)     // goes from 132 degrees to 58 degrees
    {                               
      steering.write(pos);              // writes the position to the servo
       Serial.println("Steering :"+pos);
      delay(15);                       // waits 30ms for the servo to reach the position 
    }
  }
  delay(2000); //On your marks...
  Serial.println("Delay 2000");
  steering.write(90); //get set...
  delay(100);
  throttle.write(130); //GO!
   Serial.println("throttle :130");
  delay(3000); //build up speed before the flick
  for(pos = 58; pos < 132; pos += 10)  // goes from 58 degrees (full right) to 132 degrees (full left)
  {                                   // in steps of 10 degree (fast)
    throttle.write(30);                  //BRAKE!
    steering.write(pos);              // writes the position to the servo
    delay(30);                       // waits 30ms for the servo to reach the position
  }
  for(pos = 132; pos>=58; pos-=10)     // goes from 132 degrees to 58 degrees
  {                                   
    throttle.write(170);    //Power on!
    steering.write(pos);              // writes the position to the servo
    Serial.println("Steering :"+pos);
    delay(30);    // waits 30ms for the servo to reach the position
  }
  delay(1000);                   //holds the steering at full right lock for one second
  throttle.write(140);
  Serial.println("T : 140 ");
  int maxd = 5;
  int d = 0;
  for(d = 0; d < maxd;d++){
  {
    steering.write(90);              //Sets the steering position to center
    delay(10);                       // waits 10ms for the servo to reach the position
  }
  delay(1000);                    //holds the steering at center for 1 second
  {                               
    steering.write(58);              // tell servo to go to position in variable 'pos'
    delay(30);                       // waits 15ms for the servo to reach the position
  }
  delay(500);
  }
  throttle.write(90);
  Serial.println("T : 90 ");
  steering.write(60);
  delay(60000);                   //Now Sit. Good boy.
}
