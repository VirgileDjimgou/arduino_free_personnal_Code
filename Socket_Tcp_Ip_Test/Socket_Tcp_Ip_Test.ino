
 /*
Djimgou Patrick Virgile 

 */

#include <Ethernet.h>



// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0F, 0xC4, 0x03 };
// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:

// Ip Adress der Server (TcpKnoten-Verwalter)
// IPAddress server(192, 168, 1, 104);  // Statische Adresse der Server 
// hier wird versuch , ein statische IP Adress für den Arduino festzusetzen 
IPAddress ip(192, 168, 1, 100); // diese Adress muss geandert werden , je nachdem Ihre Router konfiguration
EthernetClient Socket_client;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  
   //setup_sd_karte();
   //delay(3000);
   Setup_init_client_ethernet();

}

void loop()
{
 while (Socket_client.connected()) {
      
      // neu Eintrag in Mysql Tabelle data_a
      // Socket_client.println(neu_db_data_eintrag("Arduino_Mega_01",random(2,16),random(5,55),random(1,38),random(2.9)));
      // delay(3000);
      
      // neu Eintrag in Mysql Tabelle temperatur
      Socket_client.println(neu_de_eintrag("Arduino_Mega_01",random(0,10),random(0,50),random(0,30)));
      // Socket_client.println();
      // Socket_client.print();
      delay(3000);
      
      // echo von  knoten-Verwalter oder Server   
      while (Socket_client.available()) {
        char c = Socket_client.read();
        Serial.print(c);
         } // ende, wenn data von Server bekommen wird 
    }
    
    Serial.println("Verbindung mit dem Server unterbrochen ! ");
    // es wird hier Versuch , den Kontakt mit Server zu bauen
    Setup_init_client_ethernet();
}

