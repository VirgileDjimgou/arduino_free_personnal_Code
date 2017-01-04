
#include <SPI.h>
#include <SD.h>

File root_ver;

void setup_sd_karte() {
 
  Serial.print("Initialisierung von SD Karte...");
  if (!SD.begin(4)) {
    Serial.println("Fehler bei der Initializierung !");
    return;
  }
  Serial.println("Initializierung Erfolgreich ");

  root_ver = SD.open("/");

  Verzeichnis_Anzeiger(root_ver, 0);

}


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
    }
    test_ = entry.name();
    Serial.print(test_);
    
    if (entry.isDirectory()) {
      Serial.println("/");
      Verzeichnis_Anzeiger(entry, numTabs + 1);
    } else {
      // files have sizes, directories do not
      Serial.print("  decalage  \t\t");
      Serial.println(entry.size());
    }
    entry.close();
  }
}
