

#include <SPI.h>
#include <Ethernet.h>
#include <SPI.h>
#include <SD.h>

// set up variables using the SD utility library functions:
Sd2Card card;
SdVolume volume;

File root;
File myFile;
const int chipSelect = 4;

byte mac[] = { 0x90, 0xA2, 0xDA, 0x0F, 0xC4, 0x03 };
IPAddress server(192, 168, 1, 103);  // numeric IP for Google (no DNS)

IPAddress ip(192, 168, 1, 104);
EthernetClient client;

void setup(){
  
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

  root = SD.open("/");

  //printDirectory(root, 0);
  //read_write_send();
  Serial.println("done!");
  
  init_client_ethernet();
  delay(1000);

}
 
void loop() {
  
   //read_write_send();
   while (client.connected()) {
      //printDirectory(root, 0);
     read_write_send();
      delay(100);
      // echo from Client
      while (client.available()) {
        char c = client.read();
        Serial.print(c);
         } // end if client available
    }
    Serial.println("Client Disconnect ");
    init_client_ethernet();

}



void printDirectory(File dir, int numTabs) {
  while (true) {

    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      break;
    }
    Serial.print(entry.name());
    client.println(entry.name());
    //printDirectory(entry, numTabs + 1);
    delay(200);
    Serial.println(entry.size(), DEC);
    client.println(entry.size(), DEC);
    delay(200);
    entry.close();
    }
    
  }




void read_write_send()
     {
       
  myFile = SD.open("test.txt", FILE_WRITE);
         // re-open the file for reading:
  myFile = SD.open("test.txt");
  if (myFile) {
    Serial.println("test.txt:");
    //client.println("test.txt: ");
    delay(100);

    // read from the file until there's nothing else in it:
    char Char_File = 'q' ;
    while (myFile.available()) {
      
      //Line_File = myFile.read(); 
      Serial.write(Char_File=myFile.read());
      //Serial.write(Line_File);
      client.print(Char_File);
      delay(10);
    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
       
       
       }




void init_client_ethernet() {
  
  
  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip);
  }
  // give the Ethernet shield a second to initialize:
  delay(1000);
  Serial.println("connecting...");

  // if you get a connection, report back via serial:
  if (client.connect(server, 1001)) {
    Serial.println("connected");
    // Make a HTT request:
    client.println();
  }
  else {
    // kf you didn't get a connection to the server:
    Serial.println("connection failed");
  }
   
  }
