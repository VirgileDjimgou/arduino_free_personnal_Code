

void Tcp_Menu(){
  
      String str(Befehle);
      
      switch(Befehle[0]){
         case 'M':
                  Socket_client.println("Echo von Arduino Option : M");
                  Socket_client.println("Echo Befehle :"+Bef);
                  // instructions correspondante a M
                  break;
         case 'Z':
                  Socket_client.println("Echo von Arduino Option : Z");
                  Socket_client.println("Echo Befehle :"+Bef);
                  
                  // instruction correspondante a Z
                  break;
         case 'S':
                  Socket_client.println("Echo von Arduino Option : S");
                  Socket_client.println("Echo Befehle :"+Bef);
                  // instruction correspondante a S
                  break;
         case 'P':
                  Socket_client.println("Echo von Arduino Option : P");
                  Socket_client.println("Echo Befehle :"+Bef);
                  break;
         case 'T':
                  Socket_client.println("Echo von Arduino Option : T");
                  Socket_client.println("Echo Befehle :"+Bef);
                  break;
         case 'H':
                 Socket_client.println("Echo von Arduino Option : M");
                 Socket_client.println("Echo Befehle :"+Bef);
                 break;
         case 'W':
                 Socket_client.println("Echo von Arduino Option : W");
                  Socket_client.println("Echo Befehle :"+Bef);
                 break;
         case 'A':
                 Socket_client.println("Echo von Arduino Option : A");
                  Socket_client.println("Echo Befehle :"+Bef);
                 break;
         case 'C':
                 Socket_client.println("Echo von Arduino Option : C");
                  Socket_client.println("Echo Befehle :"+Bef);
                 break;   
         case 'D':
                 Socket_client.println("Echo von Arduino Option : D");
                  Socket_client.println("Echo Befehle :"+Bef);
                 break;                 
         case 'R':
                 Socket_client.println("Echo von Arduino Option : R");
                 Socket_client.println("Echo Befehle :"+Bef);
                 break;                  
         case 'V':
                 Socket_client.println("Echo von Arduino Option : V");
                  Socket_client.println("Echo Befehle :"+Bef);
                 break; 
         case 'X':
                 Socket_client.println("Echo von Arduino Option : X");
                  Socket_client.println("Echo Befehle :"+Bef);
                 break;
         case 'U': // Sd-Karte Operation/Funktion
                 Serial.print("Type des Befehle : U ");
                 Serial.println(Befehle);
                 root_verzeichnis = SD.open("/");
                 if(Befehle[1]=='T'){ // soll die entsprechende File nach Remote Client oder Remote Server ubertragen werden
                    
                       Serial.println("fdjhkggggg");
                       root_verzeichnis = SD.open("/");
                       int number = Befehle[2] - 48;
                       Serial.println(Befehle[2]);
                       Serial.println(number);
                       Serial.println(file_sd[number]);
                       File_Transfert(file_sd[number],0);
                    
                  }
                 
                 if(Befehle[1]=='A'){ // soll der Sd-Karte aktualisiert werden A=aktualiesieren 
                    // setup_sd();
                     root_verzeichnis.close();
                     delay(1000);
                    root_verzeichnis = SD.open("/");
                                      
                    // Verzeichnis_Anzeiger(root_verzeichnis, 0);
                    Update_Sd_karte_socket();
                 }

                 if(Befehle[1]=='L'){ // Löschen eines File auf dem SD-Karte 
                  
                  int number = Befehle[2] - 48;
                  Serial.println(number);
                  loschen_file(file_sd[number]); // file zu löschen ..... 
                  // nach loschun einer File müssen wir die Array Liste  aktualiesieren
                  
                  }
                 
                 break;

         default : {Socket_client.println("Echo von Arduino Option : Z");
                    // Socket_client.println("Echo Befehle :"+Befehle);
                    break;
                      }
            } 
       

  
  }


void Tcp_Ip_menu(char[20]){      // Auf Befehl von PC warten und dann dem entsprechend reagieren
char Head_Befehle;                       // diese Befehle werden durch TCP-Socket bekommen oder Ethernet Verbindung
int e,z,s;
    
}

void String_Extractor(){


}


// Momentan diese Funktion ist noch nicht benutz ... ich werde die später nutzen. 
char * remove_spaces(char * source, char * target)
{
     while(*source++ && *target)
     {
        if (!isspace(*source)) 
             *target++ = *source;
     }
     return target;
}


//OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO
