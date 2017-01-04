
 /*
Djimgou Patrick Virgile 

 */

#include <Ethernet.h>
#include <SPI.h>
#include <SD.h>

File root_verzeichnis;
File File_t;
char Befehle[19]=""; // 20 Charaktar zum Speicher mögliche Befehle   ....  
String file_sd[100]="         ";




// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0F, 0xC4, 0x03 };
// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:

// Ip Adress der Server (TcpKnoten-Verwalter)
IPAddress server(192, 168, 1, 102);  // Statische Adresse der Server 

// hier wird versuch , ein statische IP Adress für den Arduino festzusetzen 
IPAddress ip(192, 168, 1, 106); // diese Adress muss geandert werden , je nachdem Ihre Router konfiguration
EthernetClient Socket_client;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  
    // Socket_client = Setup_init_client_ethernet(Socket_client);
    Setup_Socket_connect();
    
    delay(500);
    setup_sd();
    delay(500);
    Socket_client.println("");
    /////////// sd Karte Inhalt senden ////////////////// 
    // Socket_client.println("Sd-Karte"); // Identifier für der Sd-karte 
    // Serial.println("sd_file=");
    Verzeichnis_Anzeiger(root_verzeichnis, 0);
    Update_Sd_karte_socket();
    // loschen_file("TEST.TXT");
    // delay(1000);
    // File_Transfert("index.txt",0);
    // Serial.println("#######################################################");
    // File_write("file_1.txt" , "information a ecrire a dans le fichier nouvellement creer");

}

void loop()
{
 while (Socket_client.connected()) {
      
      // nur zum Testen ... die muss unbedingt spater gelöscht 
      Fiktive_eintrag();
      
      // echo von  knoten-Verwalter oder Server
      int i=0;   
      while (Socket_client.available()) {
        char c = Socket_client.read();
        //Serial.print(c);

        Befehle[i]=c;
        i++;

        if((Socket_client.available()==0)){ // die geasamte char des Befehle ist auf dem TCP-Socket  Puffer Vorhanden
                   Serial.println(Befehle);
                   String Bef = Befehle ;
                   delay(500);
                   Tcp_Menu();
          }

          // 
         }
        // ende, wenn data von Server bekommen wird 
       // Serial.print("Befehle = ");
       // Serial.println(Befehle);
    
      delay(500);

         }

    Socket_client.stop(); // mussen wir wir zuert die vorhirige Verbindung schliessen und eine neue bauen
    // delay(1000);
    delay(5000); // jede 10 sekunde 
    Serial.println("Verbindung mit dem Server unterbrochen ! ");
    Setup_Socket_connect();
    // es wird hier Versuch , den Kontakt mit Server zu bauen
    // Socket_client_1 = Setup_init_client_ethernet(Socket_client_1);
}


void Fiktive_eintrag() {

  
       // random Eintrag in Remote Datenbank 
       // neu Eintrag in Mysql Tabelle data_a
          Socket_client.println(neu_db_data_eintrag("Arduino_Mega_01",random(2,16),random(5,55),random(1,38),random(2.9)));
          delay(10000);
      
      // neu Eintrag in Mysql Tabelle temperatur
         Socket_client.println(neu_de_eintrag("Arduino_Mega_01",random(0,10),random(0,50),random(0,30)));
         Socket_client.println();
        // Socket_client.print();
  
  }

