
void backup(byte btyp){
  if (btyp ==1){ //backup  Ausfallspalten anlegen
    for (int i=1; i<=ecu; i++) {
      Einlesen_BE_Matrix(i);
      Schreiben_BE_Matrix(1,2,i); 
    }
    //Serial.println("   Backup Ausfallspalten erl.  ");
  }
}  


//OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO
//OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO
void TWcount(int temp){
  if(configI[HITC]&&configI[LOWTC]){ 
    configI[TW_abs]++;          // TW die insgesammt gelaufen sind
    for (int i = 1; i <= ecu; i++) // 
    {
      if(r_ecu[i])
      {
        configI[TW_KBF_00 + i]++;       //TW_KBF_00 Startpunkt für zählen der TW
      }
    }   
    configI[HITC]=false;
    configI[LOWTC]=false;
    writeConfigI();  // ConfigI.txt anlegen und schreiben
  }
  if(temp>80){  
    configI[LOWTC]=false; // TW-Zählen nur bei T=0-Durchgang
    configI[HITC]=true;
  }
  else if(temp<(0)){    
    configI[LOWTC]=true;
  }
  if(temp>TMP_MAX){
    configI[Heatupdown]=0; // ab hier hauptsächlich Abkühlung
  }
  else if(temp<TMP_MIN){
    configI[Heatupdown]=1; // ab hier hauptsächlich Aufheizung
  }
} 

//********************************************************************************//
//                             Loop-Funktionen:                                   //
//********************************************************************************//

//OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO

void Messen(){ // einmal alle ECU's durchmesssen und Fehler in Datenbank eintragen

  long dauer;
  int aWert;
  String text="";
  char st;
  
  dauer=millis();

  // ECU
  for(int e=1;e<=ecu;e++){ // Schleife ECU
    Einlesen_BE_Matrix(e);  
   
    // Zeilen
    for(int z=1;z<=zeilen;z++){
      //Serial.println(z);
      
      Actual_Temp = temp_messen();
      delay(1);
      // Spalten
      for(int s=1;s<=spalten;s++){
        // testen der einzelen Spalten am testsubstrat, zeit war veraltet und desshalb auskommentiert
        delay(1);
        if ((BE_ok[z][s]) && (0 < r_ecu[e] < 999)) { // Nur messen wenn GS angeschlossen und noch nicht total ausgefallen ist
          aWert = Messe(e,z,s); // 10 mal an A8 messen und Mittelwert zurueckgeben
          //Serial.println(aWert);
          if (aWert > schwelle_berechnung(z,s)) { // Orginal ">" für tests "<" damit die 3 Widerstände gefunden werden können           
            if (realAusfall(e,z,s)){
              Ausfall ++;
              Ausfallhandler(e,z,s,aWert);
            }
          } // Ende if
        }//Ende überprüfung   
      }  // Ende for Spalten
    } // Ende for Zeilen
       if(Serial.available() > 0){
       st = Serial.read();
         if(st == 'A')  { 
            state = '0';
            Serial.println("Messbetrieb");

         }  
         else if(st == 'C') {
           state = '1';
           Serial.println("Kommando");
           e=31;
         }  
    }

  }  // Ende for ECU
}//Ende Messen



//OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO

void Z_Messen(int e , int z){ // einmal alle ECU's durchmesssen
  
      Einlesen_BE_Matrix(e);
      // Spalten
      for(int s=1;s<=spalten;s++){
        
          Serial.print(Messe(e,z,s));
          if(s != spalten){
            Serial.print(";");
          }
          else{
            Serial.println();
          }
      }  // Ende for Spalten
}        //Ende Z_Messen

//OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO

void S_Messen(int e , int s){ // einmal alle ECU's durchmesssen

    Einlesen_BE_Matrix(e);
    // Zeilen
    for(int z=1;z<=zeilen;z++){  

          Serial.print(Messe(e,z,s));
          if(z != zeilen){
            Serial.print(";");
          }
          else{
            Serial.println();
          }
    } // Ende for Zeilen
}//Ende S_Messen

//OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO

void P_Messen(int e){ // einmal alle ECU's durchmesssen

    Einlesen_BE_Matrix(e);
    // Zeilen
    for(int z=1;z<=zeilen;z++){
      for(int s=1;s<=spalten;s++){
          Serial.print(Messe(e,z,s));
          if((z == zeilen) && (s == spalten)){
            Serial.println();
          }
          else{
            Serial.print(";");
          } 
      }
    } // Ende for Zeilen
  //return text;  // String wahrscheinlich zu lang!
}//Ende P_Messen

//OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO

boolean realAusfall(int e, int z, int s) {
  boolean check=false;
  // hier werden Referenzmessungen gemacht
 // Serial.print("  In realAusfall: Zeile ");
  // Referenzmessung
  if((551 < Messe(1,16,32) < 571)  // Referenzmessung über Spannungsteiler zu Masse: Wert: 561 +-10
  and(554 < Messe(13,16,32) < 574) // Wert: 564 +-10
  and (Messe(e,z,s) > schwelle_berechnung(z,s))){  // nocheinmal messen, wenn erste Bedingung nicht stimmt macht er das hier gar nicht mehr!
    check = true;    // Wenn alle Messungen in Ordnung waren true returnen
  }
  Messe(e,z,s);    //  wieder richtige Adressen anlegen

  return check;
} 


//OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO

int Messe(int e, int z, int s){ // ein spezifisches BE einer alle ECU gezielt messsen und gemessenen Wert der Messspannung zurückgeben

  int ts,tz,te,aWert;

  
  if(((e % 2) == 0)){      // gerade
    digitalWrite(36,HIGH);  // normal
    digitalWrite(37,LOW);  // enable MUX gerade
  }
  else{                    // ungerade
    digitalWrite(36,LOW);   // Enable Mux ungerade
    digitalWrite(37,HIGH);   // normal
  }
  te=t_ecu[e];             // Zuordnung Messkanäle aus Zuordnungstabelle
  if (te < 16){            // Enable LOW
    digitalWrite(38,HIGH); 
    digitalWrite(39,LOW);  // getauscht
  }
  else{                    // Enable HIGH
    digitalWrite(38,LOW); 
    digitalWrite(39,HIGH);   // getauscht
  } 
  digitalWrite(40,te&1);   // Adressen fuer Kaskade
  digitalWrite(41,te&2);   // 
  digitalWrite(42,te&4);   //
  digitalWrite(43,te&8);   //
  delay(1);  
  // Zeilen
  // testen der einzelnen Zeilen.
  tz=t_zeile[z]; //Zuordnung Messkanäle aus Zuordnungstabelle

  digitalWrite(32,tz&1); // Adressen fuer Zeilen 
  digitalWrite(33,tz&2); //
  digitalWrite(34,tz&4); //
  digitalWrite(35,tz&8); //

  // Spalte
  // testen der einzelen Spalte
  ts=t_spalte[s];  // Transformieren der 64 Spalten V1..V64 in Zuordnung zu den 64 MUX-Leitungen 1..64 so wie HW-seitig angeschlossen
  if (ts < 32){               // Ansprechen MUX 1
    if (ts < 16){		    // Enable MUX 1 LOW
      digitalWrite(24,HIGH);  // 
      digitalWrite(25,LOW); 
    }
    else  {                   // Enable MUX 1 HIGH
      digitalWrite(24,LOW); 
      digitalWrite(25,HIGH);
    } 
    // Blockieren des MUX 2
    digitalWrite(30,HIGH); 
    digitalWrite(31,HIGH);
  }
  else                        // Ansprechen MUX 2
  {
    if (ts < 48){	            // Enable MUX 2 LOW
      digitalWrite(30,HIGH); 
      digitalWrite(31,LOW); 
    }
    else {    		    // Enable MUX 2 HIGH
      digitalWrite(30,LOW); 
      digitalWrite(31,HIGH);
    } 
    //Blockieren des MUX 1
    digitalWrite(24,HIGH); 
    digitalWrite(25,HIGH);
  } 	
  digitalWrite(26,ts&1);      // Adressen fuer Spalten
  digitalWrite(27,ts&2);      //
  digitalWrite(28,ts&4);      //
  digitalWrite(29,ts&8);      //
  
  //if(rueck == '1' && ( z == 1 || z == 5 || z == 7 || z == 11 || z == 15)){
    if (dir[z]) {
        digitalWrite(22,LOW);    // Zeilen (H)
        digitalWrite(23,HIGH);   // Messsignal auf LOW (Pulldown) bei Unterbrechung
      }
      else{
        digitalWrite(22,HIGH);   // Messsignal Zeilen (PULLUP)
        digitalWrite(23,LOW);    // Messsignal standartmäßig auf LOW
      }

  aWert = ReadAnalog(8);    // einlesen des Messwerts
 
 
  if((e == 1 || e == 13) && z == 16 && s == 32){
    aWert = aWert;
 } // für Referenzmessung da sonst 0 zurück gegeben wird
  else{
    if(BE_ok[z][s] == 0 || r_ecu[e] == 0 || r_ecu[e] > 999){  
      aWert = 0; // Wenn kein Teil an dieser Stelle vorhanden dann 0 zurückgeben
    }
    
  }
  return aWert;

}//Ende Messe

//OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO

int array_index(int z){      // index eines Arrays finden an dem eine bestimmte Zahl steht
  for(int i = 1;i<ecu;i++){
   if(z == r_ecu[i]){
    return i;
   } 
  }
  return -1;
}


//OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO

boolean messsignal(int a){  // legt das entsprechende Messsignal auf die Zeilen (Standardmessung mit Masse(LOW))
  if(a > 3 or a < 0){
    //Serial.println("Ungueltiges Messsignal!");
    return false;          // wenn falscher Wert return false
  }
  else{                    // Adressen anlegen und dann true returnen
    digitalWrite(44,a&2);  // B Adresse
    digitalWrite(45,a&1);  // A Adresse
    /*
     0 -> DigitalAusgang 23 (HIGH oder LOW setzen nicht vergessen!)
     1 -> 3,3V
     2 -> PWM
     3 -> nc
     */
    //Serial.println("Messsignal liegt an!");
    return true;
  }
}

//OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO

int schwelle_berechnung(byte z, byte s){    // Berechnung der Schwelle anhand der Ausfälle des Bauteils
  int sch = schwelle[z];
  sch = sch + ((BE_ok[z][s] - 1) * ((1020-schwelle[z])/(AUSFALLMAX-2))); //Berechnung der Schwelle   // AUSFALLMAX-2 -> da AUSFALLMAX-1 deltas so erreicht man auf jedenfall 1020
  // Schwelle wird mit jedem Ausfall höher gesetzt um Verhalten besser zu beobachten
  if(sch > 1020){  // Schwelle nicht höher als 1020 setzten sonst wird nichts mehr in die Datenbank eingetragen
    return 1020;
  }
  else{
  return sch;    // wenn Schwelle kleiner 1020 
   }
}

//OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO

void messen_high(int e ,int z ,int s){    // misst ein Bauteil mit High durch // danach liegt Messsignal(0) mit LOW an!!!
  int wert;    // Messwert
  boolean t;
  int save = configI[Heatupdown];  // Heatupdown speichern
  
  if(fix[z]){
  configI[Heatupdown] = 2;         // Heatupdown auf 2 setzten um zu zeigen, dass das die HIGH Messung ist
  messsignal(0);                   // Messsignal auf OUTOUT 23 setzen
  t = dir[z];                      // sichern der Richtung der Messung
  dir[z] = true;                   // Richtung auf rueckwaerts messen setzen
  wert = Messe(e,z,s);             // Messen (Adressen bleiben gleich)
  dir[z] = t;

  db_data_eintrag(e,z,s,wert);     // Datenbankeintrag machen
  
  configI[Heatupdown] = save;      // Heatupdown wiederherstellen
  }
}


//OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO

int getTW(int e){
  return configI[TW_KBF_00 + e];
}

//OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO
// Funcktion um die versammleten Daten an DatenBank Server zu senden oder die PHP-Skript ,die mit den DatenBank kommuzieren kann.

void  db_eintrag() {

  if (client.connect(serverip, 80)) {
    //Serial.println("connected");
    // Make a HTTP request:
    client.print("GET ");
    client.print(PHP_TEMPFILE);
    client.print("arduino=\"");
    client.print(ARDUINO_NAME);
    client.print("\"&temp=");
    client.print(100);
    client.print("&tcTemp=");
    client.print(Actual_Temp);
    client.print("&tw=");
    client.println(configI[TW_abs]); // TW_abs zählt absolute TW
    client.println();
    //Serial.println("Eintrag fertig");
  }
  else {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
  }  
  client.stop(); //evtl. nötig, damit in kleinen Zeitabständen DB aufrufbar
  client.flush();
}

//OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO

void  db_data_eintrag(byte e, byte z, byte s, int Analog_Wert ) {

  if (client.connect(serverip, 80)) {
    text ="ECU";
    text+= e;  
    client.print("GET ");
    client.print(PHP_DATAFILE);
    client.print("arduino=\"");
    client.print(ARDUINO_NAME);
    client.print("\"&d_id=\"");
    client.print(text);
    client.print("\"&gs_nr=");
    client.print(r_ecu[e]);// Richtige GS Nummer in Datenank eintragen durch Matrix

    client.print("&x=");
    client.print(z);
    client.print("&y=");
    client.print(s);
    client.print("&error=");
    client.print(BE_ok[z][s]);
    client.print("&tw=");
    //e++;  //reale ECU's zählen, nicht Arraystelle
    client.print(configI[TW_KBF_00 + e]); // hier muss richtiger TW-Wert in DB eingetragen werden
    client.print("&temp=");
    client.print(Actual_Temp);
    client.print("&heat=");
    client.print(configI[Heatupdown]);
    client.print("&value=");
    client.println(Analog_Wert);
    //Serial.println(" Sonstige T-Messung  Daten an kbf_data.php uebergeben");
    client.stop(); 
  } 
  else {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
  }  
  client.stop(); //evtl. nötig, damit in kleinen Zeitabständen DB aufrufbar
  client.flush();
}

//OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO
// sets every element of str to 0 (clears array)
void StrClear(char *str, char length)
{
  for (int i = 0; i < length; i++) {
    str[i] = 0;
  }
}
//OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO
// searches for the string sfind in the string str
// returns 1 if string found
// returns 0 if string not found
char StrContains(char *str, char *sfind){
  char found = 0;
  char index = 0;
  char len;

  len = strlen(str);

  if (strlen(sfind) > len) {
    return 0;
  }
  while (index < len) {
    if (str[index] == sfind[found]) {
      found++;
      if (strlen(sfind) == found) {
        return 1;
      }
    }
    else {
      found = 0;
    }
    index++;
  }

  return 0;
}

//OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO

float F_Read_TC_MK2() {
  long r;
  // unsigned char c;
  String text= "";
  float in_f;
  float ex_f;
  //== float Twert;

  uint32_t c = tc.temperatureRead();
  // Check for fault
  if(c & 0x00010000){
    // Fault detected
    Serial.print("FAULT: ");
    if (c & 0x00000001)
      Serial.println("Open Circuit");
    else if (c & 0x00000002)
      Serial.println("Short to GND");
    else if (c & 0x00000004)
      Serial.println("Short to VCC");
  }
  else{

    unsigned char in_s = !!(c & 0x00008000);
    unsigned char ex_s = !!(c & 0x80000000);
    /* Convert external temperature */
    uint16_t ext = (c >> 16 );
    ext &= (0x7FFF);
    ext >>= 2;
    ex_f = (ext >> 2) + (ext & 0x03) * 0.25;
    if(ex_s)
      ex_f = -2048 + ex_f;

    /* Convert internal temperature */
    uint16_t in = c & 0x0000FFFF;
    in >>= 4;
    in_f = (in >> 4) + (in & 0x0F) * 0.0625;
    if(in_s)
      in_s = -128 + in_s; 

    /*Serial.print("External temperature: ");
    Serial.println(ex_f, 2);
    Serial.print("Internal temperature: ");
    Serial.println(in_f, 4);*/
  }
  Actual_Temp= ex_f;
  TWcount((int)ex_f); // eigentliche Temperatur ... wird zum testen überbrückt
  //== Twert= ex_f;
  /* Convert internal temperature */
  /*  uint16_t in = r & 0x0000FFFF;
   in >>= 4;
   in_f = (in >> 4) + (in & 0x0F) * 0.0625;
   */
  //Serial.print("External temperature: ");
  //Serial.println(ex_f, 2);
  //    Serial.print("Internal temperature: ");
  //    Serial.println(in_f, 4);
  return Actual_Temp;
}

float temp_messen(){      // Temperatur 10 mal messen und Mittelwert zurückgeben 
  float wert=0;
  float maxWert=0;
  float minWert=0;
  float temp=0;

  float ibuffer[10];
  for (int i=0;i<10;i++){    
    temp = F_Read_TC_MK2();
    ibuffer[i]=temp;
    if(i==0){
      maxWert=temp;
      minWert=temp;
    }
    else{
      if(maxWert<temp){
        maxWert=temp; 
      }
      if(minWert>temp){
        minWert=temp;
      } 
    }
    wert+=temp;
  }
  wert-=maxWert;    // größter und kleinster Wert abziehen um Fehlmessungen entgegen zu kommen
  wert-=minWert;
  return (wert/8);
   
}

//OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO

int ReadAnalog(int kanal){ //Es werden an jeder Messtelle 10 Messungen gemacht und der hÃ¶chste und der niedrigste Wert ignoriert. Der Mittelwert der Ã¼brigen Messungen wird zurÃ¼ckgegeben
  int wert=0;
  int maxWert=0;
  int minWert=0;
  int temp=0;

  int ibuffer[10];
  for (int i=0;i<10;i++){    
    temp=analogRead(kanal);
    ibuffer[i]=temp;
    if(i==0){
      maxWert=temp;
      minWert=temp;
    }
    else{
      if(maxWert<temp){
        maxWert=temp; 
      }
      if(minWert>temp){
        minWert=temp;
      } 
    }
    wert+=temp;
  }
  wert-=maxWert;
  wert-=minWert;
  return (wert/8);
}

//OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO

void be_init(){ //am Anfang jedes BE, das bestückt ist messen besser aber von SD-Karte holen!! 
  byte temp;
  long dauer=0;
  //Serial.println(" delay (5 sec, dann Initialisierung BE Tabellen + SD-Karte Config anlegen)");     
  //delay(5000);
  dauer= millis();
  //Serial.println("Erstausfall anlegen ");
  //Config_BE_Save(); //nur am Anfang für Grundinitialisierung (kann wenn nur weitere Fehler registriert werden sollen auskommentiert werden)

  //delay(10000);
  dauer= millis()-dauer;
  //Serial.print("Dauer: ");
  //Serial.println(dauer);
  //delay(100000);
}



//OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO

void Config_BE_Save(){

  for (int e=1;e<=ecu;e++) { //24 ECUfiles anlegen auf SD
    Schreiben_BE_Matrix(0,0,e);  // legt die Standard .csv an
    // Serial.println("   Schreiben Konfig fertig "); // geschwindigkeitsoptimierung
  }
}


//OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO 

void Ausfallhandler(int e,int z,int s, int spannung) {
  boolean Totalausfall=false;

  if ((BE_ok[z][s]!=0) && (0 < r_ecu[e] < 999)) { // nur eintragen, wenn BE bestückt, noch kein Totalausfall und ECU angeschlossen
    BE_ok[z][s]++;
    ausfall_aktuell.e=e;
    ausfall_aktuell.z=z;
    ausfall_aktuell.s=s;
    ausfall_aktuell.anzahl_TW=configI[TW];

    ausfall_aktuell.anzahl_Ausfaelle=BE_ok[z][s];
    ausfall_aktuell.analogwert=spannung;

    Serial.print("Ausfall  ");
    Serial.print(" e ");
    Serial.print(e);
    Serial.print(" z ");
    Serial.print(z);
    Serial.print(" s ");
    Serial.print(s);
    Serial.print(" Zaehler ");
    Serial.println(BE_ok[z][s]);

    if (BE_ok[z][s]==2) { // Erstausfall, Eintragen in SD-file Erstausfaelle
      Schreiben_BE_Ausfaelle(0);  //jetzt totalausfall schreiben // vormals Erstausfall auf SD-card speichern

    }
    else if (BE_ok[z][s]>AUSFALLMAX) {
      Schreiben_BE_Ausfaelle(1);
      Serial.print("Total-Ausfall  ");
      Serial.print(" e ");
      Serial.print(e);
      Serial.print(" z ");
      Serial.print(z);
      Serial.print(" s ");
      Serial.print(s);
      Serial.print(" Zaehler ");
      Serial.println(BE_ok[z][s]);

      BE_ok[z][s]=249; // Eintrag in DB als Totalausfall
      Totalausfall=true;  // für später setzen

      // Totalausfall, Eintragen in SD-file Totalausfaelle
    }       
    // Eintrag der Daten in die DB

    db_data_eintrag(e, z, s,  spannung); //Datenbankeintrag machen
    messen_high(e,z,s);        // HIGH Messung machen und in die Datenbank eintragen
    if (Totalausfall) BE_ok[z][s]=0; // künftig nicht mehr messen;

    Schreiben_BE_Matrix(1,1,e); //File ECU_e wegschreiben
  }
  
}    

//OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO

void Einlesen_BE_Matrix(byte e) {
  String fileName="";
  String ext_T="";
  char temp;
  int i=0;

  int z=1;
  int s=1;
  String text="";

  //Serial.println("in Einlesen_Be_Matrix");

  fileName="ECU_";
  fileName+=t_ecu[e]; // hier eventuell r_ecu nehmen das .csv -Dateien richtige Bezeichnung haben!!
  //fileName+=r_ecu[e];
  fileName+=ext_T;

  fileName+=".csv";
  //Serial.print("   Einlesen BE-Matrix ");
  //Serial.print(fileName);
  //Serial.println("Dateiname fertig");

  char __dataFileName[12];
  fileName.toCharArray(__dataFileName, sizeof(__dataFileName));

  File dataFile;    
  dataFile = SD.open(__dataFileName);

  if (dataFile) {
    //Serial.println("BE-Matrix Lesen: Datei geoeffnet");   
    text="";
    while (dataFile.available()) {
      char temp=dataFile.read();
      //Serial.print("  temp  ");
      //Serial.print(temp);

      if(temp==13){ // 13 ist ASCII Carriage return (spring zum Anfang der Zeile)
        if(dataFile.read()==10) { // 10 ASCII line feed (Springt zur nächsten Zeile)        
          s++;
          i++; // zählt jedes Element des gespeicherten files          
          text=""; 
          /*Serial.print("  :  das ist Element: ");
           Serial.print(i);
           Serial.println("     EOL"); */
          //delay(10);//damit sich der Arduino nicht verschluckt
        }
      }       
      else { // kein CRLF -> richtige Zahl     
        text+=temp;      
        BE_ok[z][s]=text.toInt(); 
        //Serial.print(" BE_ok geschrieben mit: ");
        //Serial.print(BE_ok[z][s]);       
      }
      if (s>=spalten+1) {
        s=1; 
        z++;
        //Serial.println("zuruecksetzen des Spaltenzaehlers");
      }
      if (z>=zeilen+1) {
        z=1;
        //Serial.println("zuruecksetzen des Zeilenzaehlers");
      }
      // text="";       
    }  
    dataFile.close();
  }
  else {
    // if the file didn't open, print an error:
    //Serial.print("error opening file: ");
    //Serial.println(fileName);
  }
  //Serial.println("   Einlesen fertig ");
} 




//OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO 

void Schreiben_BE_Matrix(boolean initf, byte write_type, byte e) { // initf: 0: initialisierung  write_type  1: normal Schreiben 2: backup anlegen 3: HT-file 4: TT-File
  int i=1;
  String fileName="";
  String ext="";
  String ext_T="";

 
    fileName="ECU_";    // 
    fileName+=t_ecu[e]; // wie beim einlesen sollte hier dann auch r_ecu genommen werden um Dateien anzulegen!! 
    //fileName+=r_ecu[e];
    if (write_type==3) ext_T="_H";
    if (write_type==4) ext_T="_T";
    fileName+=ext_T;

    ext=".csv";
    if (write_type==2) ext=".bak";
    fileName+=ext;
    //Serial.print("   Schreiben BE-Matrix auf SD  ");
    //Serial.println(fileName);
    char __dataFileName[12];
    fileName.toCharArray(__dataFileName, sizeof(__dataFileName));

    //Orginal schreiben
    if (SD.exists(__dataFileName)) {
      SD.remove(__dataFileName);
      //   Serial.print("   ArraySave  :  remove ");
      //   Serial.println(__dataFileName);
    }
    myFile = SD.open(__dataFileName,FILE_WRITE);

    for (int z=1;z<=zeilen;z++) {
      for (int s=1;s<=spalten;s++) {
        text="";

        if (initf){// normales auslesen des Files
          text+=BE_ok[z][s];
        }    
        else{// Initialisierung des Files aus dem nachher BE_ok gelesen und gefüllt wird!
          text+=bauteil[z][s];
        }   
        myFile.println(text); 
      }
    } 
    myFile.close();
    //Serial.println("   Schreiben fertig ");
}



//OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO

void Schreiben_BE_Ausfaelle(byte write_type) {

  int i=1;
  //String ext_T="";
  String fileName="";

  char __dataFileName[12];

  /* Ausfallfile Typen:
   0: Erstausfall Originalfile
   1: Totalausfall Originalfile
   2: Erstausfall Backupfile
   3: Totalausfall Backupfile
   */
  fileName="default.csv";

  if (write_type==0){
    fileName="E_fall";
    //fileName+=ext_T;
    fileName+=".csv";
   // Serial.print("   Erstausfall  ");
  }

  if (write_type==1){
    fileName="T_fall";
    //fileName+=ext_T;
    fileName+=".csv";
    //Serial.print("   T_fall  ");    
  } 

  fileName.toCharArray(__dataFileName, sizeof(__dataFileName));

  Serial.print("   Schreiben Ausfall auf SD;  ");
  Serial.print(fileName);
  //Serial.print("   ArraySave  :  Creating ");
  //Serial.println(fileNameBackUp);
  myFile = SD.open(__dataFileName,FILE_WRITE);

  text="";
  text+=ausfall_aktuell.e;
  text+=";";
  text+=ausfall_aktuell.z;
  text+=";";
  text+=ausfall_aktuell.s;
  text+=";";
  text+=ausfall_aktuell.anzahl_TW;
  text+=";";
  text+=ausfall_aktuell.anzahl_Ausfaelle;
  text+=";";
  text+=ausfall_aktuell.analogwert;
  Serial.print(" ;  Schreiben ins File: ");
  Serial.print(text);
  myFile.println(text);

  myFile.close();

  Serial.println(" ;  Schreiben Ausfall fertig ");
}


//OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO
/*----------------------------------------------------
 * Funktion zum Schreiben des configI.txt Files auf 
 * die SD-Karte das mit werten aus dem configI[] Array
 * gefüllt wird.
 -----------------------------------------------------*/

void writeConfigI()
{
  File writeFile;  
  if (SD.exists("configI.txt"))
  {
    SD.remove("configI.txt");
  }   
  writeFile = SD.open("configI.txt", FILE_WRITE);
  if (writeFile)
  {
    //Serial.println("Folgende Werte wurden in configI[] geschrieben: ");
    for (int i= 1; i<=CONFIGILAENGE; i++) //starte die zählung der Stellen mit 1 zu besseren Verständlichkeit
    {
      if (configI[i-1] != 0)
      {
        writeFile.print(configI[i-1]);
        //Serial.println(configI[i-1]); // ausgabe was in die Datei geschrieben wird ohne Trennzeichen
      }
      else
      {
        writeFile.print('#');
        //Serial.println(configI[i-1]);
      }      
      writeFile.print(',');
    } 
    //Serial.println("configI[] in configI.txt geschrieben");   
  }
  else
  {
    //Serial.println("writeFile konnte nicht geoeffnet werden");
  }  
  writeFile.close();
}
//OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO
/*----------------------------------------------------
 * Funktion zum Einlesen des configI.txt Files das sich 
 * auf der SD-Karte befindet und abspeichern der Werte
 * in das configI[] Array.
 -----------------------------------------------------*/


void readConfigI()
{
  int i = 0;
  //int tempArr[CONFIGILAENGE];
  int tempInt = 0;
  char readByte = '0'; // read = gelesen
  File readFile; 

  if (SD.exists("configI.txt"))
  {
    readFile = SD.open("configI.txt", FILE_READ); // FILE_READ ungetestet
    if (readFile)
    {    
      while (readFile.available())  
      {
        readByte = readFile.read(); 
        //Serial.print("bytes readable: ");
        //Serial.print(readFile.available());
        //Serial.print(" Gerade gelesen");
        //Serial.println(readByte);
        if (readByte == '#') // prüfen ob ConfigI[ ] an dieser Stelle leer gewesen ist
        {
          configI[i] = 0;
          //Serial.println("war #"); // test ob in Kontrollstruktur abgezweigt wird, tut
        }
        else if (readByte == ',') // das Komma ist das Trennzeichen zwischen den Werten.
        {
          configI[i] = tempInt;
          tempInt = 0;
          i++;
          //Serial.print("tempArr["); // gibt noch komische ergebnisse wieder (der ganze block),
          //Serial.print(i);    // der gespeicherte wert stimmt aber!!!
          //Serial.print("]: ");
          //Serial.println(tempArr[i]);
        }
        else
        {
          tempInt *= 10;  // zahl wird um eine Dezimalstelle nach vorne gerückt da MSZ zuerst gelesen wird
          tempInt += ((int)readByte - 48); //ziehe alle stellen aus der ASCII tabelle ab die vor der 0 sind
        }     
        readByte='0';        
      }
      //Serial.println("configI[] erfolgreich eingelesen");
    }
    else
    {
      //Serial.println("readFile konnte nicht geöffnet werden");
    }
  }
  else
  {
    //Serial.println("configI.txt existiert nicht");
  }
  readFile.close();  
}
