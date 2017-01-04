
// #include <SPI.h>
// #include <SD.h>

File root_verzeichnis;
File File_t;

void setup_sd() {
 
  Serial.print("Initialzirung der Arduino Sd-Karte...");

  if (!SD.begin(4)) {
    Serial.println("Fehler bei der Initializierung !");
    return;
  }
  Serial.println("Initializierung Erfolgreich .");

  root_verzeichnis = SD.open("/");

  Verzeichnis_Anzeiger(root_verzeichnis, 0);

}

// diese Funktion dient zum Auflisten der Daten , die auf denm Arduino-Karte derzeit vorhanden sind
void Verzeichnis_Anzeiger(File dir, int numTabs) {
  String test_;
  while (true) {

    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print('\t');
      // Socket_client.print('\t');
      delay(50);
    }
    test_ = entry.name();
    Serial.print(test_);
    // Socket_client.print(test_);
    delay(50);
    if (entry.isDirectory()) {
      Serial.println("/");
      // Socket_client.println("/");
      delay(50);
      Verzeichnis_Anzeiger(entry, numTabs + 1);
    } else {
      // files have sizes, directories do not
      Serial.print("  \t\t");
      Serial.println(entry.size());
      // Socket_client.print("  \t\t");
      delay(50);
      // Socket_client.println(entry.size());
      delay(50);
    }
    entry.close();
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
