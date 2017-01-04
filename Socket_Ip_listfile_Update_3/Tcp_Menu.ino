

void Tcp_Menu(){
  
      
      switch(Befehle[0]){
         case 'M':
                  Serial.print("Type des Befehle : M ");
                  Serial.println(Befehle);
                  // instructions correspondante a M
                  break;
         case 'Z':
                  Serial.print("Type des Befehle : Z ");
                  Serial.println(Befehle);
                  // instruction correspondante a Z
                  break;
         case 'S':
                  Serial.print("Type des Befehle : S ");
                  Serial.println(Befehle);
                  // instruction correspondante a S
                  break;
         case 'P':
                  Serial.print("Type des Befehle : P ");
                  Serial.println(Befehle);
                  break;
         case 'T':
                  Serial.print("Type des Befehle : T ");
                  Serial.println(Befehle);
                  break;
         case 'H':
                 Serial.print("Type des Befehle : H ");
                 Serial.println(Befehle);
                 break;
         case 'W':
                 Serial.print("Type des Befehle : W ");
                 Serial.println(Befehle);
                 break;
         case 'A':
                 Serial.print("Type des Befehle : A ");
                 Serial.println(Befehle);
                 break;
         case 'C':
                 Serial.print("Type des Befehle : C ");
                 Serial.println(Befehle);
                 break;   
         case 'D':
                 Serial.print("Type des Befehle : D ");
                 Serial.println(Befehle);
                 break;                 
         case 'R':
                 Serial.print("Type des Befehle : R ");
                 Serial.println(Befehle);
                 break;                  
         case 'V':
                 Serial.print("Type des Befehle : V ");
                 Serial.println(Befehle);
                 break; 
         case 'X':
                 Serial.print("Type des Befehle : X  ");
                 Serial.println(Befehle);
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

         default : Serial.println("ungultige Befehle eingekommen ! ");
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
