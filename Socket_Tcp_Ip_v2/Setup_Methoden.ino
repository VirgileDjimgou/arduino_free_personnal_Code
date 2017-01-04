
//********************************************************************************//
//                             SETUP-Funktionen:                                  //
//********************************************************************************//


//***************************************************************************************//
//***************************************************************************************//
//                                 FUNKTIONEN                                            //
//***************************************************************************************//
//***************************************************************************************//

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
  if (client.connect(serverip, 700)) {
    Serial.println("connected");
    // Make a HTT request:
    client.println();
  }
  else {
    // kf you didn't get a connection to the server:
    Serial.println("connection failed");
  }
  
  // Initialisierung von Server .... für den File Handling wird es benutz
  server.begin();
  
  
  }


void EthernetConnection(){
  client = server.available();  // try to get client
  if (client) {  // got client?
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {   // client data available to read
        char c = client.read(); // read 1 byte (character) from client
        // buffer first part of HTTP request in HTTP_req array (string)
        // leave last element in array as 0 to null terminate string (REQ_BUF_SZ - 1)
        if (req_index < (REQ_BUF_SZ - 1)) {
          HTTP_req[req_index] = c;          // save HTTP request character
          req_index++;
        }
        //Serial.print(c);    // print HTTP request character to serial monitor
        // last line of client request is blank and ends with \n
        // respond to client only after last line received
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connnection: close");
          client.println();
          // open requested web page file
          if (StrContains(HTTP_req, "GET / ")
            || StrContains(HTTP_req, "GET /index.htm")) {
            webFile = SD.open("index.htm");        // open web page file
          }
          else if (StrContains(HTTP_req, "GET /page2.htm")) {
            webFile = SD.open("page2.htm");        // open web page file
          }
          else if (StrContains(HTTP_req, "GET /DATA2.CSV")) {
            webFile = SD.open("DATA2.CSV");        // open web page file
          } 
          else if (StrContains(HTTP_req, "GET /config1.csv")) {
            webFile = SD.open("config1.csv");        // open web page file
          } 
          else if (StrContains(HTTP_req, "GET /matrix.csv")) {
            webFile = SD.open("matrix.csv");        // open web page file
          }
          // send web page to client
          if (webFile) {
            while(webFile.available()) {
              client.write(webFile.read());
              //Serial.print(".");
            }
            webFile.close();
          }
          // reset buffer index and all buffer elements to 0
          req_index = 0;
          StrClear(HTTP_req, REQ_BUF_SZ);
          break;
        }
        // every line of text received from the client ends with \r\n
        if (c == '\n') {
          // last character on line of received text
          // starting new line with next character read
          currentLineIsBlank = true;
        } 
        else if (c != '\r') {
          // a text character was received from client
          currentLineIsBlank = false;
        }
      } // end if (client.available())
    } // end while (client.connected())
    delay(1);      // give the web browser time to receive the data
    client.stop(); // close the connection
  } // end if (client)
}

//OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO
//OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO

void SetupEthernet(){
  Ethernet.begin(mac, ip);
  server.begin();
  //Serial.print("server is at ");		//Ausgabe der eigenen IP auf dem COM-Port
  //Serial.println(Ethernet.localIP());
}

//OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO
//OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO

void SetupTimer(){
  //init Timer2
  TCCR2B  = (1<<CS22); //clk=F_CPU/64
  TCNT2   = 0x00;
  TIMSK2 |= (1<<TOIE2); //enable overflow interupt
  //interrupts on
  sei();
}


//OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO
//OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO
//------------------------------------------
// Organisation der Ausgangspins, Belegung mit 
// default Potential und Richtung 
// 
//------------------------------------------
void SetupPin(){

  pinMode(22,OUTPUT);    // Messsignal Zeilen (PULLUP-PULLDOWN)  
  pinMode(23,OUTPUT);    // Messsignal Spalten
  
  pinMode(24,OUTPUT);    // enable MUX Spalten 1 LOW
  pinMode(25,OUTPUT);    // enable MUX Spalten 1 HIGH
  pinMode(26,OUTPUT);    // S0 Spalten
  pinMode(27,OUTPUT);    // S1 Spalten
  pinMode(28,OUTPUT);    // S2 Spalten
  pinMode(29,OUTPUT);    // S3 Spalten
  pinMode(30,OUTPUT);    // enable MUX Spalten 2 LOW
  pinMode(31,OUTPUT);    // enable MUX Spalten 2 HIGH
  pinMode(32,OUTPUT);    // A_Zeile
  pinMode(33,OUTPUT);    // B_Zeile
  pinMode(34,OUTPUT);    // C_Zeile
  pinMode(35,OUTPUT);    // D_Zeile
  pinMode(36,OUTPUT);    // ungerade_Zeile
  pinMode(37,OUTPUT);    // gerade_Zeile

  pinMode(A8,INPUT);     // Messsignal Zeilen (J2,Kaskade)

  pinMode(38,OUTPUT);    // Enable_LOW_Kaskade
  pinMode(39,OUTPUT);    // ENABLE_HIGH_Kaskade
  pinMode(40,OUTPUT);    // A_Kaskade
  pinMode(41,OUTPUT);    // B_Kaskade
  pinMode(42,OUTPUT);    // C_Kaskade
  pinMode(43,OUTPUT);    // D_Kaskade

  pinMode(44,OUTPUT);    // B_Messsignal
  pinMode(45,OUTPUT);    // A_Messsignal


  //Pre-set
  digitalWrite(22,HIGH);    // Messsignal Zeilen (PULLUP) 
  digitalWrite(23,LOW);     // Messsignal standardmäßig auf LOW

  digitalWrite(24,HIGH);   // Alle Adressen auf LOW und Enables auf inaktiv
  digitalWrite(25,HIGH);
  digitalWrite(26,LOW);
  digitalWrite(27,LOW);
  digitalWrite(28,LOW);
  digitalWrite(29,LOW);
  digitalWrite(30,HIGH);
  digitalWrite(31,HIGH);
  digitalWrite(32,LOW);
  digitalWrite(33,LOW);
  digitalWrite(34,LOW);
  digitalWrite(35,LOW);
  digitalWrite(36,HIGH);
  digitalWrite(37,HIGH);

  digitalWrite(38,HIGH);  //
  digitalWrite(39,HIGH);  //
  digitalWrite(40,LOW);  //
  digitalWrite(41,LOW);  //
  digitalWrite(42,LOW);  // 
  digitalWrite(43,LOW);  //  

  messsignal(0);        // Messsignal auf 1. Signal legen -> Pin 23 (LOW)
  // Messsignale in messsignal() beschieben

  //Serial.println("setup pins beendet");
}


//OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO
//OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO
void SetupSD(){
  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);
  // initialize SD card
  //Serial.println("Initializing SD card...");
  if (!SD.begin(4)) { // Pin 4 ist beim Ethernet Shield der ChipSelect(CS)
    Serial.println("ERROR - SD card initialization failed!");
    return;    // init failed
  }
  //Serial.println("SUCCESS - SD card initialized.");
  // check for index.htm file
  if (!SD.exists("index.htm")) {
    //Serial.println("ERROR - Can't find index.htm file!");
    return;  // can't find index file
  }
  //Serial.println("SUCCESS - Found index.htm file.");
}

//OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO
//---------------------------------------------------------
// Initialisierung von Bauteil mit den der Bestückung der
// KleinBauFormen entsprechendenwerten 1 == Bauteil 
// exitiert 0 == an dieser stelle kein Bauteil
//---------------------------------------------------------

void initBauteil(){
  for(int z = 1; z <= zeilen; z++) // Zeilenzähler  
  {
    for (int s = 1; s <= spalten; s++) // Spaltenzähler
    {
      bauteil [z][s] = 1;
    }
  }
  //es gibt 6 nicht belegte Plätze pro Seite in der BauteilMatrix der KBF
  bauteil[8][16] = 0;
  bauteil[8][17] = 0;
  bauteil[7][16] = 0;
  bauteil[7][17] = 0;
  bauteil[9][16] = 0;
  bauteil[9][17] = 0;
  bauteil[8][48] = 0;
  bauteil[8][49] = 0;
  bauteil[7][48] = 0;
  bauteil[7][49] = 0;
  bauteil[9][48] = 0;
  bauteil[9][49] = 0;
  //Serial.println("Initialisierung bauteil [][] abgeschlossen.");
}


//OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO
//----------------------------------------------------
// Initialisieren von configI[] mit Null
//-----------------------------------------------------
void SetupConfigI(){
  if (SD.exists("configI.txt") && !CONFIG_REMOVE)// bei CONFIG_REMOVE = 0 soll der altes tw-stand behalten werden
  {
    readConfigI(); 
    //Serial.println("Initialisierung von configI[] durch lesen von configI.txt");   
  }
  else
  {
    if (CONFIG_REMOVE) // Wenn CONFIG_REMOVE 1, also true ist, sollen die alten TW-Stände gelöscht werden
    {
      SD.remove("configI.txt");
    }
    for (int i = 0; i < CONFIGILAENGE ; i++)
    {
      configI[i] = 0;
    }
    //Serial.println("configI[] Initialisiert");
    
    // hier TW der KBF eintragen
    // config[TW_KBF_XX] = xxx;
    //verspannte -10/+90°C
 /*   configI[TW_KBF_5] =  1080;   
    configI[TW_KBF_10] = 1080;    
    configI[TW_KBF_15] = 1080;   
    configI[TW_KBF_20] = 1080;   
    configI[TW_KBF_24] = 1080;   

    // unverspannte -10/+90°C
    configI[TW_KBF_19] = 3036;
    configI[TW_KBF_4] = 3036;
    configI[TW_KBF_14]= 3036;
    configI[TW_KBF_27]= 3036;
    configI[TW_KBF_23]= 3036;
    configI[TW_KBF_9]= 3036; 
    Serial.println("Hardcodewerte in configI[] eingetragen"); 
    */ 
    
    writeConfigI(); // configI[] auf SD-Karte schreiben
  }
  
}
