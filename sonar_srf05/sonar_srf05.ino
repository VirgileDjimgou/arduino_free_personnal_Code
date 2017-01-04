/*
 * rosserial SRF08 Ultrasonic Ranger Example
 *
 * This example is calibrated for the SRF08 Ultrasonic Ranger.
 */
 
const int TRIG_PIN = 12;
const int ECHO_PIN = 13;  

//#include <Sonar_srf08.h> //SRF08 specific library
//#include <WProgram.h>
#include <Wire.h>
#include <ros.h>
#include <std_msgs/Float32.h>


//Set up the ros node and publisher
std_msgs::Float32 sonar_msg1;
std_msgs::Float32 sonar_msg2;
std_msgs::Float32 sonar_msg3;
ros::Publisher pub_sonar_1("sonar_1", &sonar_msg1);
ros::Publisher pub_sonar_2("sonar_2", &sonar_msg2);
ros::Publisher pub_sonar_3("sonar_3", &sonar_msg3);
ros::NodeHandle nh;


float sensorReading =0;

char unit = 'c'; // 'i' for inches , 'c' for centimeters


void setup()
{
  
  nh.initNode();
  nh.advertise(pub_sonar_1);
  nh.advertise(pub_sonar_2);
  nh.advertise(pub_sonar_3);
  
  // sonar 1
  pinMode(12,OUTPUT);
  pinMode(13,INPUT);
  
  // sonar 2
   pinMode(2,OUTPUT);
  pinMode(3,INPUT);
  
  // sonar 3
   pinMode(7,OUTPUT);
  pinMode(6,INPUT);

}

void loop()
{
  delay(20);
  sonar_msg1.data = distance(12,13);
  pub_sonar_1.publish(&sonar_msg1);
  delay(20);
  sonar_msg2.data = distance(2,3);
  pub_sonar_2.publish(&sonar_msg2);
  delay(20);
  sonar_msg3.data = distance(7,6);
  pub_sonar_3.publish(&sonar_msg3);
  nh.spinOnce();
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
