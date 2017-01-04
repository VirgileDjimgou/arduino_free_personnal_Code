 /* 
 * rosserial IR Ranger Example  
 * 
 * This example is calibrated for the Sharp GP2D120XJ00F.
 */

#include <ros.h>
#include <Servo.h>
#include <ros/time.h>
#include <sensor_msgs/Range.h>
#include <std_msgs/UInt16.h>

Servo servo;
Servo servo_2;

ros::NodeHandle  nh;
sensor_msgs::Range range_msg;
ros::Publisher pub_range( "range_data", &range_msg);

void servo_cb( const std_msgs::UInt16& cmd_msg){
  servo.write(cmd_msg.data); //set servo angle, should be from 0-180  
  
}

void servo_2_call( const std_msgs::UInt16& cmd_msg){
  servo_2.write(cmd_msg.data); //set servo angle, should be from 0-180   
}

void Motoren_Link( const std_msgs::UInt16& cmd_msg){
  
  analogWrite(3, cmd_msg.data); 
  digitalWrite(12, HIGH-digitalRead(12));  //toggle led  
}

void Motoren_Recht( const std_msgs::UInt16& cmd_msg){
  
  analogWrite(11, cmd_msg.data); 
  digitalWrite(13, HIGH-digitalRead(13));  //toggle led  
}

ros::Subscriber<std_msgs::UInt16> sub_M_L("M_L", Motoren_Link);
ros::Subscriber<std_msgs::UInt16> sub_M_R("M_R", Motoren_Recht);
ros::Subscriber<std_msgs::UInt16> sub("s1", servo_cb);
ros::Subscriber<std_msgs::UInt16> sub_2("s2", servo_2_call);

const int analog_pin = 0;
unsigned long range_timer;

/*
 * getRange() - samples the analog input from the ranger
 * and converts it into meters.  
 */
float getRange(int pin_num){
    int sample;
    // Get data
    sample = analogRead(pin_num)/4;
    // if the ADC reading is too low, 
    //   then we are really far away from anything
    if(sample < 10)
        return 254;     // max range
    // Magic numbers to get cm
    sample= 1309/(sample-3);
    return (sample - 1)/100; //convert to meters
}

char frameid[] = "/ir_ranger";

void setup()
{ 
  pinMode(11, OUTPUT); // pwm  B
  pinMode(13, OUTPUT); // dir B
  pinMode(3, OUTPUT); // pwm A
  pinMode(12, OUTPUT); // dir A
  pinMode(13, OUTPUT);

  nh.initNode();
  nh.subscribe(sub_M_L); // Initialierung  linke Motoren 
  nh.subscribe(sub_M_R); // Initialisierung Rechte Motoren
  nh.subscribe(sub);  
  nh.subscribe(sub_2); 
  servo.attach(10); //attach it to pin 10
  servo_2.attach(6); //attach it to pin 6
  
  
  nh.advertise(pub_range); 
  range_msg.radiation_type = sensor_msgs::Range::INFRARED;
  range_msg.header.frame_id =  frameid;
  range_msg.field_of_view = 0.01;
  range_msg.min_range = 0.03;
  range_msg.max_range = 0.4;
  
}

void loop()
{
  // publish the range value every 50 milliseconds
  //   since it takes that long for the sensor to stabilize
  if ( (millis()-range_timer) > 50){
    range_msg.range = getRange(analog_pin);
    range_msg.header.stamp = nh.now();
    pub_range.publish(&range_msg);
    range_timer =  millis();
  }
  nh.spinOnce();
  delay(1);
}
