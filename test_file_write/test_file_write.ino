#include <SPI.h>
#include <SD.h>


void setup() {
  // put your setup code here, to run once:
   // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }


  Serial.print("Initializing SD card...");

  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
  
  delay(1000);
  test_funktion();

}

void loop() {
  

}

void test_funktion(){

  char fileContents[128]; // Probably can be smaller
   byte index = 0;
   File dataFile = SD.open("file_1.txt");
   
   if (dataFile) {
   while (dataFile.available()) 
   {
      char aChar = dataFile.read();
      if(aChar != '\n' && aChar != '\r')
      {
         fileContents[index++] = aChar;
         fileContents[index] = '\0'; // NULL terminate the array
      }
      else // the character is CR or LF
      {
         // Serial.print("fileContents: [");
         // Serial.print(fileContents);
         // Serial.println("]");
         if(strlen(fileContents) > 0)
         {
            Serial.println(fileContents);
            float aVal = atof(fileContents);
            // Serial.println(index);
            // Do something with aVal
         }

         fileContents[0] = '\0';
         index = 0;
      }
    }
   }
   else{ Serial.println( " Fehler ! " ); }


}
