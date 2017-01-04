/*
Djimgou Patrick Virgile 

 */
 

void Setup_init_client_ethernet() {
  
  
  // Start mit der  Ethernet Verbindung:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");

    Ethernet.begin(mac, ip);
  }
  // give the Ethernet shield a second to initialize:
  delay(1000);
  Serial.println("connecting...");

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

