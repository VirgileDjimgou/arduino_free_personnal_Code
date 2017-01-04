//#include <WProgram.h>
#include <Servo.h>
#include <ros.h>
#include <std_msgs/Float64.h>

#define STBY 6
#define B_PWM 3
#define A_PWM 9
#define A_IN2 8
#define A_IN1 7
#define B_IN1 5
#define B_IN2 4

ros::NodeHandle nh;

float servoL, servoR;
Servo ServoL;
Servo ServoR;

int commandL = 0;
int commandR = 0;

void messageCbl( const std_msgs::Float64& msg){

servoL = msg.data;
commandL = servoL;
if(commandL == 0.0)
{
digitalWrite(B_IN1, LOW); //
digitalWrite(B_IN2, LOW); //
digitalWrite(B_PWM, HIGH); //
digitalWrite(STBY, HIGH); // STYBY - HIGH
}
else if (commandL > 0)
{
digitalWrite(B_IN2, HIGH);
digitalWrite(B_IN1, LOW);
digitalWrite(STBY, HIGH); // STYBY - HIGH
analogWrite(B_PWM, commandL);
}
else
{
digitalWrite(B_IN2, LOW);
digitalWrite(B_IN1, HIGH);
digitalWrite(STBY, HIGH); // STYBY - HIGH
analogWrite(B_PWM, -commandL);
}
}

void messageCbr( const std_msgs::Float64& msg){
servoR = msg.data;
commandR = servoR;
if(commandR == 0.0)
{
digitalWrite(STBY, HIGH); // STYBY - HIGH
digitalWrite(A_IN1, LOW);
digitalWrite(A_IN2, LOW);
digitalWrite(A_PWM, HIGH);
}
else if(commandR > 0)
{
digitalWrite(A_IN2, LOW);
digitalWrite(A_IN1, HIGH);
digitalWrite(STBY, HIGH); // STYBY - HIGH
analogWrite(A_PWM, commandR);
}
else
{
digitalWrite(A_IN2, HIGH);
digitalWrite(A_IN1, LOW);
digitalWrite(STBY, HIGH); // STYBY - HIGH
analogWrite(A_PWM, -commandR);
}
}

ros::Subscriber<std_msgs::Float64> sl("servoleft", &messageCbl);
ros::Subscriber<std_msgs::Float64> sr("servoright", &messageCbr);
void setup(){
pinMode(13, OUTPUT); // LED Indicator
pinMode(A_IN1, OUTPUT); // AIN2
pinMode(A_IN2, OUTPUT); // AIN1
pinMode(STBY, OUTPUT); // STNBY
pinMode(B_IN1, OUTPUT); // BIN1
pinMode(B_IN2, OUTPUT); // BIN2
pinMode(A_PWM, OUTPUT); //
pinMode(B_PWM, OUTPUT); //

nh.initNode();
nh.subscribe(sl);
nh.subscribe(sr);

digitalWrite(B_IN2, LOW);
digitalWrite(B_IN1, LOW);
digitalWrite(B_PWM, LOW);
digitalWrite(STBY, LOW); // STYBY - HIGH
digitalWrite(A_IN1, LOW);
digitalWrite(A_IN2, LOW);
digitalWrite(A_PWM, LOW);
}

void loop(){
nh.spinOnce();
delay(20);
}
