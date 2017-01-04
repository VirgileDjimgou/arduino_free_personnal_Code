
// prototype menu nur zum Testphase ... später muss diese gelöscht werden
void prototype_menu(){
   
   while (client.connected()) {
      
      // neu Eintrag in Mysql Tabelle data_a
      // Socket_client.println(neu_db_data_eintrag("Arduino_Mega_01",random(2,16),random(5,55),random(1,38),random(2.9)));
      // delay(3000);
      
      // neu Eintrag in Mysql Tabelle temperatur
      client.println(neu_de_eintrag("Arduino_Mega_01",random(0,10),random(0,50),random(0,30)));
      delay(3000);
      
      // echo von  knoten-Verwalter oder Server   
      while (client.available()) {
        char c = client.read();
        Serial.print(c);
         } // ende, wenn data von Server bekommen wird 
    }
    
    Serial.println("Verbindung mit dem Server unterbrochen ! ");
    // es wird hier Versuch , den Kontakt mit Server zu bauen
    Setup_init_client_ethernet();
   

}


//OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO

void Tcp_Ip_menu(void){      // Auf Befehl von PC warten und dann dem entsprechend reagieren
char bc;                       // diese Befehle werden durch TCP-Socket bekommen oder Ethernet Verbindung
int e,z,s;
     /*if (cmd_ok== false) {
       Serial.println("cmd ok");
       cmd_ok= true;
     }
     */
     
     // hier wird ein spezialle Menu durch TCP
     
     /*
     while(Serial.available() <= 0);    // auf Befehl warten
       bc = Serial.read();
       switch(bc){
         case 'M':
                  e = Serial.parseInt();
                  z = Serial.parseInt();
                  s = Serial.parseInt();
                  if(array_index(e) == -1){
                    break;
                  }
                  Einlesen_BE_Matrix(array_index(e));
                   
                  Serial.println(Messe(array_index(e),z,s));  // Entsprechende ECU, Spalte und Zeile Messen   
                  break;
         case 'Z':
                  e = Serial.parseInt();
                  z = Serial.parseInt();
                  if(array_index(e) == -1){
                    break;
                  }
                  Z_Messen(array_index(e),z);  // Entsprechende Zeile auf ECU messen
                  break;
         case 'S':
                  e = Serial.parseInt();
                  s = Serial.parseInt();
                  if(array_index(e) == -1){
                    break;
                  }
                  S_Messen(array_index(e),s);  // Entsprechende Spalte auf ECU Messen
                  break;
         case 'P':
                  e = Serial.parseInt();
                  if(array_index(e) == -1){
                    break;
                  }
                  P_Messen(array_index(e));  // Entsprechende ECU ganz messen
                  break;
         case 'T':
                 Serial.println( (int)temp_messen());
                 break;
         case 'H':
                 Serial.println(configI[Heatupdown]);
                 break;
         case 'W':
                 e = Serial.parseInt();
                 if(array_index(e) == -1){
                    break;
                  }
                 Serial.println(getTW(array_index(e)));
                 break;
         case 'A':
                 state = '0';
                 Serial.println("Messbetrieb");
                 break;
         case 'C':
                 state = '1';
                 Serial.println("Kommando");
                 break;   
         case 'D':
                 for (int i=1; i<=16; i++) {
                    dir[i] = Serial.parseInt();
                    Serial.print(dir[i]);
                    Serial.print(" ,");
                 }
                  Serial.println("");
                 break;                 
         case 'R':
                 rueck = '0';
                 Serial.println("R");
                 break;                  
         case 'V':
                 rueck = '1';
                 Serial.println("V");
                 break; 
         case 'X':
                 Serial.print("sw_Stand");
                 Serial.println(sw_Stand);
                 Serial.print("Commandmode: ");
                 Serial.print(state);
                 Serial.print("  Messmode r/v (0/1): ");
                 Serial.println(rueck);
                 Serial.print("  1,16,32 : ");
                 Serial.println(Messe(1,16,32));
                 Serial.print("  13,16,32 : ");
                 Serial.println(Messe(13,16,32));
                 break;
       } 
       
       */
}


//OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO

void serielle_menu(void){      // Auf Befehl von PC warten und dann dem entsprechend reagieren
char bc;                       // diese Befehle werden durch TCP-Socket bekommen oder Ethernet Verbindung
int e,z,s;
     /*if (cmd_ok== false) {
       Serial.println("cmd ok");
       cmd_ok= true;
     }
     */
     while(Serial.available() <= 0);    // auf Befehl warten
       bc = Serial.read();
       switch(bc){
         case 'M':
                  e = Serial.parseInt();
                  z = Serial.parseInt();
                  s = Serial.parseInt();
                  if(array_index(e) == -1){
                    break;
                  }
                  Einlesen_BE_Matrix(array_index(e));
                   
                  Serial.println(Messe(array_index(e),z,s));  // Entsprechende ECU, Spalte und Zeile Messen   
                  break;
         case 'Z':
                  e = Serial.parseInt();
                  z = Serial.parseInt();
                  if(array_index(e) == -1){
                    break;
                  }
                  Z_Messen(array_index(e),z);  // Entsprechende Zeile auf ECU messen
                  break;
         case 'S':
                  e = Serial.parseInt();
                  s = Serial.parseInt();
                  if(array_index(e) == -1){
                    break;
                  }
                  S_Messen(array_index(e),s);  // Entsprechende Spalte auf ECU Messen
                  break;
         case 'P':
                  e = Serial.parseInt();
                  if(array_index(e) == -1){
                    break;
                  }
                  P_Messen(array_index(e));  // Entsprechende ECU ganz messen
                  break;
         case 'T':
                 Serial.println( (int)temp_messen());
                 break;
         case 'H':
                 Serial.println(configI[Heatupdown]);
                 break;
         case 'W':
                 e = Serial.parseInt();
                 if(array_index(e) == -1){
                    break;
                  }
                 Serial.println(getTW(array_index(e)));
                 break;
         case 'A':
                 state = '0';
                 Serial.println("Messbetrieb");
                 break;
         case 'C':
                 state = '1';
                 Serial.println("Kommando");
                 break;   
         case 'D':
                 for (int i=1; i<=16; i++) {
                    dir[i] = Serial.parseInt();
                    Serial.print(dir[i]);
                    Serial.print(" ,");
                 }
                  Serial.println("");
                 break;                 
         case 'R':
                 rueck = '0';
                 Serial.println("R");
                 break;                  
         case 'V':
                 rueck = '1';
                 Serial.println("V");
                 break; 
         case 'X':
                 Serial.print("sw_Stand");
                 Serial.println(sw_Stand);
                 Serial.print("Commandmode: ");
                 Serial.print(state);
                 Serial.print("  Messmode r/v (0/1): ");
                 Serial.println(rueck);
                 Serial.print("  1,16,32 : ");
                 Serial.println(Messe(1,16,32));
                 Serial.print("  13,16,32 : ");
                 Serial.println(Messe(13,16,32));
                 break;
       } 
}

