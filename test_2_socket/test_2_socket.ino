
#include <SPI.h>
#include <Ethernet.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0x90, 0xA2, 0xDA, 0x0F, 0xC4, 0x03
};
IPAddress ip(192, 168, 1, 102);

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(80);

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}


void loop() {
  // listen for incoming clients
  Serial.println("wiat");
  EthernetClient client = server.available();
  Serial.println("wait client");
  if (client) {
    Serial.println("new client");
    boolean currentLineIsBlank = true;
    while (client.connected()) {
       if (client.available()) {
         // char c = client.read();
         Serial.println("hgjhgjfgjhgff fffffffrrr");
         // Serial.write(c);
         delay(100);
         client.println("ht_ht_ht_ht_ht_ht_ht_fluo_fluo_htiuoiujoi");
         delay(100);
         client.println("Conjuhjhjh");
     
      }
    }
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
    Ethernet.maintain();
  }
}
