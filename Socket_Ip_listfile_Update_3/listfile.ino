
// #include <SPI.h>
// #include <SD.h>



int File_auf_SD=0; 
void setup_sd() {
 
  Serial.print("Initializierung der Arduino Sd-Karte...");

  if (!SD.begin(4)) {
    Serial.println("Fehler bei der Initializierung !");
    return;
  }
  Serial.println("Initializierung Erfolgreich .");

  root_verzeichnis = SD.open("/");

  // hier werden wir der Inhalt des SD anzeigen  .... 
  // aber muss die nicht unbedingt hier sein
 

}

// SD.open("/");
void printDirectory(File dir, int numTabs) {
   while(true) {
     
     File entry =  dir.openNextFile();
     if (! entry) {
       // no more files
       break;
     }
     for (uint8_t i=0; i<numTabs; i++) {
       Serial.print('\t');
     }
     Serial.print(entry.name());
     if (entry.isDirectory()) {
       Serial.println("/");
       printDirectory(entry, numTabs+1);
     } else {
       // files have sizes, directories do not
       Serial.print("\t\t");
       Serial.println(entry.size(), DEC);
     }
     entry.close();
   }
}


// diese Funktion dient zum Auflisten der Filen , die auf denm Arduino-Karte derzeit vorhanden sind
void Verzeichnis_Anzeiger(File dir, int numTabs) {
  String test_;
  int i=0;
  int list_file=0;
  
  // Socket_client.println("Inhalt Sd-Karte : ");
  file_sd[100]="         "; // Init Sd file liste
  while (true) {

    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print('\t');
      Socket_client.print('\t');
      delay(10);
    }
    test_ = entry.name();
    file_sd[list_file]=test_;
    list_file ++;
  if (entry.isDirectory()) {
      Serial.println("/");
      Socket_client.println("/");
      delay(10);
      Verzeichnis_Anzeiger(entry, numTabs + 1);
    } else {
      Serial.println("sd_file="+ test_ +"     >>    "+entry.size()+" kb");
      Socket_client.println("sd_file="+ test_ +"     >>   "+entry.size()+" kb");
      delay(10);
      // files have sizes, directories do not
      //Serial.print("  \t\t");
      //Serial.println(entry.size());
      //Socket_client.print("  \t\t");
      //delay(50);
      //Socket_client.println(entry.size());
      //delay(50);
    }
    entry.close();
  }
 File_auf_SD=list_file; 
}

void file_sd_aktualisierung (int file_number) {
  
  }
  
void Update_Sd_karte_socket(){
  
   for (int k = 0; k < File_auf_SD + 1 ; k ++) {

       Serial.println(file_sd[k]);
       Socket_client.println(file_sd[k]);
         
           }

       }

// neue Funktion zum Loschen unötige File 
void  loschen_file(String File_Name_String){
     
      char filename_char[100];
      File_Name_String.toCharArray(filename_char, sizeof(filename_char));
      
        
      //Serial.println("Loschen des File " + File_Name + "...");
      SD.remove(filename_char);
    
      if (SD.exists(filename_char)) {
        Serial.println(File_Name_String + "ist vorhanden .");
      }
      else {
        Serial.println(File_Name_String + " wurde auf dem Sd-Karte gelöscht ! ");
      }
 
   }
  
  
// neu Funktion zur Erstelleung neuen Files 
void file_erstellung(String File_Name_String){
  
      char filename_char[100];
      File_Name_String.toCharArray(filename_char, sizeof(filename_char));
        // zur Erstellung eine neu File 
      Serial.println("Creating File "+ File_Name_String +"...");
      File_t = SD.open(filename_char, FILE_WRITE);
      File_t.close();
      // wir mussen zuerst prüfen , ob diese File auf dem Sd-karte existiert !
      
      if (SD.exists(filename_char)) {
        Serial.println(" "+File_Name_String+ " ist vorhanden ");
      }
      else {
       
        Serial.println(" "+File_Name_String+ " ist nicht vorhanden ");
      }

  }
  
 
  
  /// nicht funktionfähig momentan
  void File_write(String file, String neu_daten){
    
        char filename_char[100];
        file.toCharArray(filename_char, sizeof(filename_char));
        // hier mussen  die String irgendwie konvertieren .... und dann 
        
        File dataFile = SD.open(filename_char , FILE_WRITE);
      
        // if the file is available, write to it:
        if (dataFile) {
          Serial.print("Schreiben in  File " + file + " ...");
          dataFile.println(neu_daten);
          dataFile.close();
          // print to the serial port too:
          Serial.println(neu_daten);
        }
        // if the file isn't open, pop up an error:
        else {
          Serial.println("Fehler beim Offnen " + file);
        }
        
  }
  
  
// FileName : name der File zu senden 
// type der Operation : Kopien oder auschneiden (kopie und cut )
//type  0 = Kopie
//type  1 = cut
void File_Transfert(String file,int type){
  
   root_verzeichnis = SD.open("/");
   char fileContents[128]; // Probably can be smaller
   byte index = 0;
   char filename_char[50];
   file.toCharArray(filename_char, sizeof(filename_char));
   File dataFile = SD.open(filename_char);

   // Socket_client.println(file);
   // Serial.println(file);
   if (dataFile) {
    // wir mussen davor die Filename senden und  Inthalt der File danach
    Socket_client.print("Name der File : ");
    Socket_client.println(file);
    Socket_client.println("              ");
    
   while (dataFile.available()) 
   {
      char aChar = dataFile.read();
      if(aChar != '\n' && aChar != '\r')
      {
         fileContents[index++] = aChar;
         fileContents[index] = '\0'; // NULL terminate the array
      }
      else // the character is CR or LF
      {  //  wenn die Line etwas enthalt ... nicht leer ist  dann 
         if(strlen(fileContents) > 0)
         {
            Serial.println(fileContents);
            Socket_client.println(fileContents);
            delay(20);
            float aVal = atof(fileContents);
            // Serial.println(index);
            // Do something with aVal
         }

         fileContents[0] = '\0';
         index = 0;
      }
    }
    dataFile.close();
   }
   else{ Serial.println( " Fehler ! " ); }
   dataFile.close();
}

void Test_File(char filename_char[19],int type){

   char fileContents[128]; // Probably can be smaller
   byte index = 0;
   //char filename_char[50];
   //file.toCharArray(filename_char, sizeof(filename_char));
   Serial.print("file zu suchen :");
   Serial.println(filename_char);
   Serial.print("end");
   File dataFile = SD.open(filename_char);
   
   if (dataFile) {
    // wir mussen davor die File Name senden und der enthalt der File danach
    Socket_client.print("Name der File : ");
    Socket_client.println(filename_char);
    Socket_client.println("              ");
    
   while (dataFile.available()) 
   {
      char aChar = dataFile.read();
     
   }
   
}else{ Serial.println( "Keine File unter diese Name ! " ); }
}
