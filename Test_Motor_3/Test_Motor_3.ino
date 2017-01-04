#include <Servo.h>

/* Servo configuration */
Servo steeringServo;
Servo frontServo;
Servo rearServo;

/* Defines current speed for both axis
   0 = 100% forward, 100 = stop (none), 200 = 100% backward
*/
int motorSpeed = 100;

/* Defines current steering angle in percent 
    0 = full left, 100 = center, 200 = full right
*/
int steeringAngle = 100;

/* initial set-up of the unit */
void setup(){  
  steeringServo.attach(3);
  frontServo.attach(11);
  rearServo.attach(10);
  
  /* set up the serial with 9600 baud */
  Serial.begin(115200);
}

/* loop function for its operation */
void loop(){      
  /* check for commands from serial */
  if(Serial.available()>0){
    /* read command from serial */
    byte buffer[3];
    Serial.readBytes((char*)buffer,3);
    Serial.print("b0 :");
    Serial.println(buffer[0]);
    Serial.println(sizeof(buffer));
    
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
    
    /* send current status through serial */
    byte outBuffer[3];
    outBuffer[0] = (byte)steeringAngle;
    outBuffer[1] = (byte)motorSpeed;
    outBuffer[2] = (byte)222;
    Serial.write(outBuffer,3);
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
    rearServo.write(90);
  }if(value>0&&value<101){
    /* drive forward */
    int driveAngle = (int)(90+(70*(float)((float)value/100)));
    frontServo.write(driveAngle);
    rearServo.write(driveAngle);
  }if(value>100&&value<201){
    /* drive backward */
    int driveAngle = (int)(90-(70*(float)(((float)value-100)/100)));
    frontServo.write(driveAngle);
    rearServo.write(driveAngle);
  }
  
  /* update the local variable */
  motorSpeed = value;
}

/* sets the steering in percent */
void setSteeringPercentage(int value){
  /* Steering angles:
    50 = full right
    90 = center
    130 = full left
  */
  Serial.println("Steeringpercent :"+value);
  if(value==0){
    steeringServo.write(90);
  }if(value>0&&value<101){
    /* steer left */
    int leftAngle = (int)(90+(40*(float)((float)value/100)));
    steeringServo.write(leftAngle);
  }if(value>100&&value<201){
    /* steer right */
    int rightAngle = (int)(90-(40*(float)(((float)value-100)/100)));
    steeringServo.write(rightAngle);
  }
  
  /* update the local variable */
  steeringAngle = value;
}
