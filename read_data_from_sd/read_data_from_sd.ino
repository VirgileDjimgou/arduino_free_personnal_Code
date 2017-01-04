// Read Data from SD with Arduino http://zygzax.com/
 
//We include the SD library.
#include <SD.h>
// We store in what Arduino input the pin CS of the module is connected.
const int chipSelect = 4;
 
void setup(){
  // We set up the serial port to send error messages through It.
  Serial.begin(9600);
   
  // The default CS pin of the Arduino board must be configured as an output
  // even when not used (10 in most of the boards, 53 in Arduino Mega).
  pinMode(10, OUTPUT);
   
  // If there has been an error reading the SD card we send a message through the serial port.
  if (!SD.begin(chipSelect)){
    Serial.println("Error reading the SD card.");
    return;
  } 
 
  //We write the program inside the setup to only run it once.
 
  // We open the file.
  File dataFile = SD.open("readme.txt");
  // If the file is correct:
  if (dataFile) {
    // We show a message saying that the content of the txt file starts below.
    Serial.println("* The content of readme.txt is shown below:");
    // We read the file and send the data through the serial port.
    while (dataFile.available()) {
      Serial.write(dataFile.read());
    }
    // We close the file.
    dataFile.close();
  // If we could not open the file we send an error message through the serial port.
  }else {
    Serial.println("Error opening readme.txt");
  }
}
 
//We Don't write anything in the loop.
void loop()
{ 
}
