
/* Neuorganisation des Programms mit folgende Zielen
- Reduktion aller features auf Notwendiges
- neue Referenztabellen: pro pcb 1 File editierbar B_n.txt (n...PCB-Nummmer) erste 10bit=Referenzwert, restl. 6 bit (evtl.auch integer-File binär als .dat parallel zum Test)
- Berechnung jeweiliger Fehlermaxima aus Polynomkurve und RefWert bei RT
- 
- 

*/
#include <inttypes.h>

// include the SD library:
#include <SD.h>

// dise Librarys werden eigentlich nicht hier gebraucht !... das  heißt  können wir  die als kommentar  umbenan
// #include <SDcard.h> 
// SDcard sd;
/*
Die Kommunikation zwischen dem Mikrocontroller und der SD Karte nutzt SPI, das sich an den Pins 11, 12 und 13 (auf den meisten Arduino Boards) 
 oder 50, 51 und 52 (Arduino Mega), befindet. Zusätzlich muss ein anderer Pin genutzt werden um die SD Karte auszuwählen. 
 Dafür kann der Hardware SS Pin (Pin 10 auf den meisten Boards) oder Pin 53 (beim Mega) - oder ein anderer Pin genutzt werden. 
 Der zu verwendende Pin muss beim Aufruf von SD.begin() festgelegt werden.
 Speicherrelevante Stringersetzungen durch Päprüzessoranweisungen innerhalb der entsprechenden Funktionen direkt am Anfang 
 vorgenommen. Hierbei gilt das der Funktionsname der Anfang des zugehörigen Symbols ist.
 */
String sw_Stand="feldnah_cmd1"; //feldnahe Hübe neues Grundprogramm

//include  Ethernet

#include <SPI.h>
#include <Ethernet.h>

//include Display
//#include <MI0283QT9.h> 
// #include <ADS7846.h>

#include <Wire.h>

//include  MOD-TC-MK2 Thermoelementmodul Olimex
#include <TC-MK2.h>
TC_MK2 tc(0x23);

// define size of buffer used to capture HTTP requests
#define REQ_BUF_SZ   20

// **************************** Konfigurationsdaten *****************************

#define TEMP1 1        // Inhalte des arrays configI bezeichnen mit verständlichen Namen
#define TEMP2 2
#define TEMP3 3
#define HITC 4
#define LOWTC 5
#define TW 6
#define Heatupdown 7
#define TW_abs 8        // Zählt absolute TW seit Start

//#define TW_KBF_00 XX   -> Startwert  //
//#define TW_KBF_XX XX   -> nicht bestückt!! 
// erste MUX-Platine
// ab hier richtet es sich nach ecu anzahl = anzahl(#defines TW_KBF_XX)
#define TW_KBF_00 9       // Start Wert zum zählen der TW  // #defines werden im Prinzip nicht benötigt sind nur zur Übersichlichkeit!
                           //          e auf MUX-Platine!
//#define TW_KBF_XX 10     //          e = 01  Referenzmessung
//#define TW_KBF_XX 11     //          e = 02 // verspannte -10/+90°C
#define TW_KBF_5 12     //          e = 03     5
#define TW_KBF_10 13     //          e = 04    10
#define TW_KBF_15 14     //              05    15
#define TW_KBF_20  15     //              06    20
#define TW_KBF_24 16     //              07    24
//#define TW_KBF_xx 17     //              08
//#define TW_KBF_xx 18     //              09 
//#define TW_KBF_xx 19     //              10
//#define TW_KBF_26 20     //              11 
//#define TW_KBF_XX 21     //              12

// zweite MUX-Platine
//#define TW_KBF_XX  22     //          e = 01       // freie PCB -10/+90°C
//#define TW_KBF_XX  23     //          e = 02
#define TW_KBF_19  24     //          e = 03
#define TW_KBF_4 25     //              04
#define TW_KBF_14 26     //              05
#define TW_KBF_27 27     //              06
#define TW_KBF_23 28     //              07
#define TW_KBF_9 29     //              08
#define TW_KBF_XX 30     //              09
#define TW_KBF_XX 31     //              10
#define TW_KBF_XX 32     //              11
#define TW_KBF_XX 33     //              12

#define CONFIG_REMOVE 0 // WICHTIG!!! bei 1 werden die auf der SD gepeicherten TW-Stände gelöscht VORSICHT!!!
#define CONFIGILAENGE 40

#define PHP_TEMPFILE "/kbf/kbf_temperatur.php?" // in diesem .php-file auf dem Server steht die Ziel-Tabelle zum Eintragen der Temperatur
#define PHP_DATAFILE "/kbf/kbf_data.php?" // in diesem .php-file auf dem Server steht die Ziel-Tabelle zum Eintragen der Temperatur
#define ARDUINO_NAME "RT-X6560" // 11 L= Labor | 

#define TMP_MAX 90
#define TMP_MIN -10
#define AUSFALLMAX 12  // von 10 auf 12, so dass 1020 auf jedenfall erreicht wird  // mind. 3!! da sonst Berechnung einen Fehler bringt

// set up variables Config
int configI[CONFIGILAENGE]; // Speichern der Konfiguration vor evtl. Programmabsturz auf SD
const int spalten = 64; // Anzahl der Spalten MUX 4067 zum Anlegen der Messpannung
const int zeilen = 16;  //Matrix MUX 4067 zum Durchschalten der Messpannung (von I/O 22 und 23 an analog inputs)
const int ecu = 24;     //Anzahl der ansteuerbaren ECU's(im Moment nur 12, sonst 24) //(am Ende werden wir nur 17 benötigen)
boolean dir[zeilen + 1] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; // alle Zeilen auf Rückwärtsrichtung messen, d.h. bein Ausfall -> 1023
boolean fix[zeilen + 1] = {0,1,0,0,0,1,0,1,0,0,0,1,0,0,0,1,0};  // gilt für Layout KBF1: MOSFET's werden zusätzlich rueckwaerts gemessen werden

char state = '2';    // var für Umschalten des Programms 0 = Messbetrieb; 1 = Kommando durch Serielle Schnistelle ;
                     // 2 = Kommando duch Tcp-Ip (Ethernet) Schnistelle
                     
char rueck = '0';    // var für das Umschalten der Messrichtung bei den FETs

const int intervallTempDB=5; // Eintrag Temperatur jede 6 min in Datenbank (-> 3x2 min)
const int intervallsaveSD= 30; // Sicherung Ausfalldaten alle 30 min auf SD-Karte
const int intervall_backup= 1000; // Sicherung ca. alle 20h 

// ab hier Integration Programmsegment "matrix_ok" 


const int schwelle[zeilen + 1] = {
      0,  950,620,620,620,950,650,950,650,630,650,950,650,630,650,950,650}; // bei Werten groesser ist Kontaktstelle defekt!
// dummy, 001,002,003,004,005,006,007,008,009,010,011,012,013,014,015,016

// Zuordnung von 64 Spalten (im Layout V1..V64) und 16 Zeilen (im Layout H1..H) zu MUX-Adressen

const byte t_spalte[spalten+1]={
  0,  55,54,53,52,51,50,49,47,32,33,34,35,36,37,38,39,8,9,10,11,12,13,14,15,0,1,2,3,4,5,6,7,
  40,41,42,43,44,45,46,48,63,62,61,60,59,58,57,56,23,22,21,20,19,18,17,16,31,30,29,28,27,26,25,24};//*/

//Stelle 0  im Array unbelegt, damit Zeile/Spalte 1 auch im Array mit [1] adressiert

const byte t_zeile[zeilen+1]={
  0,  15,7,14,6,13,5,12,4,11,3,10,2,9,1,8,0};
// Transformations-Matrix für Zeilen: Layout <-> MUX-Adresse

const byte t_ecu[ecu+1]={
  0,  30,2,29,3,28,4,27,5,26,6,25,7,   20,12,19,13,18,14,17,15,16,0,31,1 /* ,8,23,9,22,10,21,11*/}; // ECU's

const int r_ecu[ecu+1] = { // (0 = false, >0 = true)
  0, 1000,0, 5,10,15,20,24,0,0,0,0,0  ,1001,0,19,4,14,27,23,9,0,0,0,0}; // Eintragen welche ECU auf welchem Stecker der MUX Platine steckt, wenn nichts steckt -> 0!
// wird für richtigen Datenbankeneintrag benutzt/gebraucht und TWcount(), messen() und ausfallhaendler()

boolean bauteil[zeilen+1][spalten+1] ;// Initialisierung erfolgt durch initBauteil 

//eingabe
byte BE_ok[zeilen+1][spalten+1]; // array für Ausfallzähler mit Koordinagen aus ECU-Nummer und Zeilen-nummer

struct record
{
  byte e;  
  byte z;
  byte s;
  int anzahl_TW;
  int anzahl_Ausfaelle;
  int analogwert;
};
typedef struct record Buffer;
Buffer ausfall_aktuell; // in diesem record werden alle zu einem Ausfall relevanten Daten übergeben

int Ausfall=0;
int fehler[ecu];    //
int Actual_Temp;   //globale Variable für aktuelle Temperatur
String text="";

static uint16_t count2=600;// Zähler für ISR
static uint16_t count3=0;  // Zeitkonstante für Abspeichern auf SD-Karte
static uint16_t count4=0;
static uint16_t count5=0;

boolean countb=false;

//variables using the files
char __dataFileName[12];
char __dataFileNameB[12];
String fileNameBackUp;

// set up variables using the Ethernet

File webFile,myFile,myFile2,myFile1;                    // handle to files on SD card
char HTTP_req[REQ_BUF_SZ] = {
  0}; // buffered HTTP request stored as null terminated string
char req_index = 0;              // index into HTTP_req buffer

// set up variables using the SD utility library functions:
Sd2Card card;
SdVolume volume;
SdFile root;
const int chipSelect = 4;    //SD Karte
#if _USE_LFN //use LFN (long file names)
char LFName[_DF1S ? ((_MAX_LFN*2)+1) : (_MAX_LFN+1)]; //LFN buffer
#endif
//Interrupt Service Routine (ISR) dient zur Taktung von Temperatureintragungen in DB und für backup auf SD
ISR(TIMER2_OVF_vect){ //Timer 2 Interrupt, called every xms
  static uint8_t count=1;
  TCNT2 -= 250; //1000 Hz orig 250 (in Ref. 131)
  if(--count == 0){ //100 Hz
    count = 10;
    if(--count2 == 0){ // Hz
      count2 = 3000; // 1 Messung pro Min. bei TCNT2=250 und count2=3000 
      countb=true; // beim nächsten Durchlauf durch loop Temperatur messen
    }
  }
}

// Initialisierung der Tcp_Ip  Komponenten 
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0F, 0xC4, 0x03 };//Arduino Büro  //hier noch die richtige mac eintragen
// hier wird versuch , ein statische IP Adress für den Arduino festzusetzen 
IPAddress ip(192, 168, 1, 100); // diese Adress muss geandert werden , je nachdem Ihre Router konfiguration
// IPAddress ip(10,38,186,108);       // ..186.. für Labor-Netz
//IPAddress ip(10,38,182,124); // ..182.. Büro-Netz 
//      byte mac[] = {0x90, 0xA2, 0xDA, 0x0F, 0x62, 0x3D }; 

// Ip Adress der Server (TcpKnoten-Verwalter)
IPAddress serverip(192, 168, 1, 104);  // Statische Adresse der Server 

// IPAddress serverip(10,38,182,102); // Adresse des Severs mit MySQL-Datenbank
// Initialize the Ethernet server library with the IP address and port you want to use.
// (port 80 is default for HTTP):
EthernetClient client; // Socket Client 
EthernetServer server(80);


void setup(){
  //********************************************************************************
  //                             Pins I/O für HW Matrix setzen
  //********************************************************************************
  SetupPin(); 
  //
  //********************************************************************************
  //                             Serial
  // Open serial communications and wait for port to open:
  //********************************************************************************
  Serial.begin(115200);
  //Serial.println("serial ok"); 
  //********************************************************************************
  //                             ETHERNET
  //********************************************************************************
  // SetupEthernet(); 
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  } 
   Setup_init_client_ethernet();
  //********************************************************************************
  //                            TIMER
  //********************************************************************************
  SetupTimer();
  //********************************************************************************
  //                             SD Karte
  //********************************************************************************
  //Serial.println("setup SD in 5 sec"); 
  //delay(5000);
  SetupSD();
  //********************************************************************************
  //                             TW-Zyklen
  //******************************************************************************** 
  SetupConfigI();
  // Prüfung ob die festgelegten Werte verwendet werden oder vorher schon welche gespeichert wurden

  //********************************************************************************
  //                             I2C
  //********************************************************************************
  Wire.begin();
  //Serial.println("Wire.begin()"); 

  //********************************************************************************
  // Initialisieung des Arrays bauteil[][] nur bestückte BE mit 1, unbestückte in der Mitte mit 0
  // DIESE FUNKTION MUSS VOR "be_init()" AUFGERUFEN WERDEN!
  //********************************************************************************
  initBauteil();
  //
  be_init();
  Serial.println("X"); // wenn Monitor aktiv einzige Rückmeldung vom Programm nach setup vor loop
}
//***************************************************************************************//
//                                 LOOP                                                  //
//***************************************************************************************//
void loop(void) {

  long dauer;
  long zWert;
    
  int s,z,e, aWert;  //temporäre Variablen fuer Tests verwendet
  int messwert;
  char st;

  if (countb){  // Interrupt ist abgelaufen -> Temperaturmessung
    count3++; //intervallsaveSD) 
    count4++;  // Temperatur in DB eintragen
    count5++;// Intervall für backaup auf SD
    countb=false; 
    Actual_Temp = temp_messen(); // aktuelle Temperatur messen und an globale Variable übergeben
    //Serial.print("Actual_Temp: ");
    //Serial.println(ActuaIntervall für l_Temp);

    if(count3> intervallsaveSD) { //intervallsaveSD) 
      count3=0; // alle 60 min (T-Messungen) Abspeichern der Daten auf SD-Karte
      //MatrixSave(); ausblenden solange nicht mit SD-Karte ausgerüstet und funktional sonst ist Menü ohne Zeichen
    }
    if(count4> intervallTempDB) {
      count4=0;  // alle 5 min (T-Messungen) Abspeichern der Daten in Datenbank 
      //Actual_Temp = F_Read_TC_MK2(); // aktuelle Temperatur messen
      db_eintrag();
    }
    if(count5> intervall_backup) {
      count5=0;  //   jeden Tag 1x Backup der Datenfiles  
      backup(1); //backup  Ausfallspalten anlegen 1x pro Tag = 240 x6min
      Serial.println(" Interrupt backup Intervall durchfuehren"); 
    }  
  } 
  EthernetConnection(); // Funktion zur Etablierung LAN-Anschluss an Server
  
  //------------------------------------------------------------------------
  Actual_Temp = temp_messen(); // aktuelle Temperatur messen und an globale Variable übergeben
 
  if(state == '0'){ // Messmodus für eigenständige Messungen
    Messen(); // Messroutine
  }
  else if(state == '1'){  // Command-Modus: Arduino tauscht über ser. Schnittstelle mit Master-Rechner Daten aus
   serielle_menu(); // Auswertung der Kommando-Befehle durch serielle Schnittstelle
  }
  else if(state == '2'){ // Command-Modus : Arduino tauscht über Tcp_IP (Ethernet Anschlusse) mit Server oder Master Rechner Daten aus 
   prototype_menu(); // Auswertung der Kommando-Befehle durch Tcp-Ip Ethernet Schnittstelle
  }
}





