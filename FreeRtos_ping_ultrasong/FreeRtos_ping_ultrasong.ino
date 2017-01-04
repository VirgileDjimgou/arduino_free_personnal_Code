
#include <NewPing.h>
#include <FreeRTOS_AVR.h>

#include <ros.h>
#include <ros/time.h>
#include <sensor_msgs/Range.h>

ros::NodeHandle  nh;

sensor_msgs::Range range_msg;
ros::Publisher pub_range( "/ultrasound", &range_msg);


#define SONAR_NUM     2 // Number of sensors.
#define MAX_DISTANCE 200 // Maximum distance (in cm) to ping.
#define PING_INTERVAL 33 // Milliseconds between sensor pings (29ms is about the min to avoid cross-sensor echo).

NewPing sonar_1(6,7, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
NewPing  sonar_2(11,12,MAX_DISTANCE);

static void Thread1(void* arg) {
  while (1) {
     // Sleep for 200 milliseconds.
    vTaskDelay((1000L * configTICK_RATE_HZ) / 1000L);
     Serial.println("Thread 1 ..... ");
     Serial.print(sonar_1.ping_cm()); // Send ping, get distance in cm and print result (0 = outside set distance range)
     Serial.println("cm");
     
  }
}

//  Declare the thread function for thread 2.
static void Thread2(void* arg) {
  while (1) {
     // Sleep for 200 milliseconds.
    vTaskDelay((2000L * configTICK_RATE_HZ) / 1000L);
    Serial.println("Thread 2 ..... ");
    Serial.print(sonar_2.ping_cm()); // Send ping, get distance in cm and print result (0 = outside set distance range)
    Serial.println("cm");
  
  }
}

void setup() {
   portBASE_TYPE s1, s2;
  Serial.begin(115200);
 
  // initialize semaphore
  // sem = xSemaphoreCreateCounting(1, 0);

  // create task at priority two
  s1 = xTaskCreate(Thread1, NULL, configMINIMAL_STACK_SIZE, NULL, 1, NULL);

  // create task at priority one
  s2 = xTaskCreate(Thread2, NULL, configMINIMAL_STACK_SIZE, NULL, 2, NULL);

  // check for creation errors
  if (s1 != pdPASS || s2 != pdPASS ) {
    Serial.println(F("Creation problem"));
    while(1);
  }
  // start scheduler
  vTaskStartScheduler();
  Serial.println(F("Insufficient RAM"));
  while(1);
    
}

void loop() {
 
  // Other code that *DOESN'T* analyze ping results can go here.
}

