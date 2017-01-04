/*
Djimgou Patrick Virgile 

 */
 

EthernetClient  Setup_init_client_ethernet(EthernetClient Socket_instanz) {
  EthernetClient Soc_client;
  
  Serial.println("setup Funktion");
  // Start mit der  Ethernet Verbindung:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");

    Ethernet.begin(mac, ip);
  }
  // give the Ethernet shield a second to initialize:
  delay(500);
  Serial.println("connecting...");

  // if you get a connection, report back via serial:
  if (Socket_instanz.connect(server, 700)) {
    Serial.println("connected");
    // Make a HTT request:
    // Socket_client.println("");
    EthernetClient Soc_client = Socket_instanz;
    Serial.println(Socket_instanz.connected());
  }
  else {
    // kf you didn't get a connection to the server:
    Serial.println("connection failed");
    Serial.println(Socket_instanz.connected());
  }
  
  return Soc_client;
  }



  void Setup_Socket_connect() {
  
  Serial.println("setup Funktion");
  // Start mit der  Ethernet Verbindung:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");

    Ethernet.begin(mac, ip);
  }
  // give the Ethernet shield a second to initialize:
  delay(500);
  Serial.println("connecting...");

  // EthernetClient Socket_client;
  // if you get a connection, report back via serial:
  if (Socket_client.connect(server, 700)) {
    Serial.println("connected");
    // Make a HTT request:
    Socket_client.println();
  }
  else {
    // kf you didn't get a connection to the server:
    Serial.println("connection failed");
  }
  
  
  }

