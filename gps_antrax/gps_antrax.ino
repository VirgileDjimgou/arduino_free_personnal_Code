/* Filename: GSM_HTTP.ino
 * Author: Sam Albuquerque
 * Description: Sketch to periodically upload GPS data over GPRS
 *              to remote Server via HTTP connection
 * used with Arduino Mega
 */


#include <SPI.h>
#define baud 9600

//Class to handle the GPS part of the shield.
class GPS {
  public:
    GPS(int ledPin);
    char coordinate[23];  //DDMM.SSSSN,DDDMM.SSSSW
    boolean gpsState;  
    void getGPS();
    long time;            //HHMMSS  UTC time
  private:
    int gpsLed;           //PIN Number for GPS LED
};


GPS::GPS(int ledPin){
  gpsLed=ledPin;
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin,LOW);
  gpsState = false;
  time=0;
}

void GPS::getGPS(){
  char gps_data_buffer[7];
  char command_result;
  byte high_Byte;
  int i;
  byte GPGGA;

  SPI.begin();
  SPI.setDataMode((0<<CPOL) | (1 << CPHA) |(1<<SPI2X));

  delay(1000);

  GPGGA=0;
  i=0;
  do {
    digitalWrite(SS, LOW);
   
    if(high_Byte == 1){
      command_result=SPI.transfer(0xA7);
      high_Byte = 0;
    } else {
      command_result=SPI.transfer(0xB4);
      high_Byte = 1;
    }
    for(int index=0; index<6; index++){
      gps_data_buffer[index] = gps_data_buffer[index + 1];
    }
    gps_data_buffer[6] = command_result;

    if((gps_data_buffer[0] == '$') && (gps_data_buffer[1] == 'G') && (gps_data_buffer[2] == 'P') && (gps_data_buffer[3] == 'G') && (gps_data_buffer[4] =='G') && (gps_data_buffer[5] == 'A')) {
      GPGGA = 1;         //We found a $GPGGA Sentence
    }

    if((GPGGA == 1) && (i < 80 ))
    {

      if(gps_data_buffer[0] == 0x0D) { //Sentence ends with 0x0D(Return) Character
        i = 80;
        GPGGA = 0;
      } else {
        if(i>=7 && i<13){   //Characters 7-12 are UTC Time String
          time = ((time*10) + (int(gps_data_buffer[0]) - 48))%1000000;
        }
        if(i>=18 && i<27){  //Characters 18-26 are Latitude
         coordinate[i-18]=gps_data_buffer[0];
        }
        if(i==28) coordinate[9]=gps_data_buffer[0];  //Character 28 is either N or S Hemisphere
        if(i==29) coordinate[10]=',';       
        if(i>=30 && i<40){  //Characters 30 to 39 are Longitude
         coordinate[i-19]=gps_data_buffer[0];
        }        
        if(i==41) coordinate[21]=gps_data_buffer[0];  
        i++;                //Character 41 is either E or W Hemisphere
        }
    }

    digitalWrite(SS, HIGH);
  } while (i<80);
  if((coordinate[0]) == ','){
      gpsState=false;          //GPS Position Not Found
  } else {
    gpsState=true;            //GPS Position Found
    coordinate[22]='\0';      //Valid String ends with Null Character.
  }
  digitalWrite(gpsLed, gpsState);
  return;
}

GPS gps(9);                          //Pin 9 is used to control the GPS LED.

char PIN[5] = "0000";                // replace this with your SIM PIN
char cmd;                            // command read from terminal
long lastTime=0;

void setup() {          
  Serial.begin(9600);
  Serial.println("GM862 monitor");
}


void loop() {
    Serial.println("G monitor");

    gps.getGPS();
    Serial.println(gps.coordinate);      
    //Prints 0 if GPS fix not received.
    delay(3000);
    
    if(gps.gpsState==true){
       
           Serial.println(gps.coordinate);              //Prints 0 if GPS fix not received.

     } 
}
