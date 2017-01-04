#include <TimedAction.h>
#include <Ethernet.h>
#include <TC_MK2.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
TC_MK2 tc(0x23);

EthernetClient socket;
File filePointer;
String buff = "";
String name;
int refNormValueMin = 0;
int refNormValueMax = 0;
int port = 8888;
byte mac[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
byte serverAdr[] = { 0, 0, 0, 0 };
byte ip[] = { 0, 0, 0, 0 };
byte zuordnungLogPhys[32];
byte castResultShort[4];
byte castResultLong[6];
int maxPlattenNummer;
int plattenNummer;
double startTemp = 0;
double temp = 0;
byte nextJob = 0;
byte maxErr = 0;
byte job = 0;
byte state = 0;
char startTempTrend = '0';
char tempTrend = '0';
char newJob = 'f';
double oldTemp = 555.55;
char tcDone = 't';

//###################################################################################################################
//################################################### Zeit-Events ###################################################
//###################################################################################################################
void sendStatusCheck() {
  if (state == 0) {
    sendStatus();
    while (!socket.connected() && job != 0) {
      socket.flush();
      socket.stop();
      socket.connect(serverAdr, port);
      send("init:" + name);
      job=1;
      sendStatus();
    }
    measureTemp();
    send("temp<br>" + ((String)temp) + "<br>" + tempTrend);
  }
}
TimedAction jobStatusAction = TimedAction(5000, sendStatusCheck);
char routine() {
  while (!socket.connected() && job != 0) {
    socket.flush();
    socket.stop();
    socket.connect(serverAdr, port);
    send("init:" + name);
    job=1;
    sendStatus();
  }
  if (socket.available() > 0) {
    parser(socket.readString());
  }
  jobStatusAction.check();
  return newJob;
}
//###################################################################################################################
//####################################################### Main ######################################################
//###################################################################################################################
void setup() {
  Serial.begin(9600);
  Serial.println("Start");
  setupPin();
  state = 1;
  plattenNummer = -1;
  if (loadConfig()) {
    //Serial.print("Connect to the server [");
    for (int i = 0; i < 4; i++)
      //Serial.print(((String)serverAdr[i]) + ".");
      //Serial.println("]");
      Ethernet.begin(mac, ip);
    //Serial.println("Ethernet ready!");
    if (socket.connect(serverAdr, port)) {
      state = 0;
      //Serial.println("Connected!");
      send("init:" + name);
      job = 1;
      sendStatus();
    }
  } else {
    //Serial.println("Fatal error");
    state = 1;
    while (1);
  }
}
void loop() {
  if (state != 1) {
    newJob = 'f';
    routine();
    switch (job) {
      case 0:
        setJobStatus(1);
        break;
      case 1:
        break;
      case 2:
        measurementMode(true);
        break;
      case 3:
        measurementMode(false);
        break;
      case 4:
        break;
      default:
        sendError("Job:" + job);
        break;
    }
  } else {
    //Serial.println("Fatal error");
    while (1);
  }
}
//###################################################################################################################
//################################################## Kommunikation ##################################################
//###################################################################################################################
void parser(String msg) {
  switch (msg[0]) {
    case 'b':
      setJobStatus(1);
      break;
    case 'k':
      if (job != 4) {
        plattenNummer = -1;
        maxPlattenNummer = -1;
        state = 2;
        if (msg.indexOf("v") != -1) {
          nextJob = 2;
          msg.replace("k>v>", "");
        } else {
          nextJob = 3;
          msg.replace("k>s>", "");
        }
        //Serial.println("Get:"+msg);
        maxErr = msg.substring(0, msg.indexOf('>')).toInt();
        //Serial.println("MaxErrString:"+msg.substring(0, msg.indexOf('>')));
        //Serial.println("MaxErr:"+((String)maxErr));
        msg.remove(0, msg.indexOf('>') + 1);
        //Serial.println("rest:"+msg);
        byte counter = 0;
        String puffer = "";
        for (int c = 0; c < msg.length(); c++) {
          if (msg[c] == '<') {
            zuordnungLogPhys[counter] = puffer.toInt();
            break;
          } else if (msg[c] == '>') {
            zuordnungLogPhys[counter] = puffer.toInt();
            puffer = "";
            counter++;
          } else {
            puffer += msg[c];
          }
        }
        //Serial.println("Next Job: " + ((String)nextJob));
        setJobStatus(4);
      } else {
        byte nextIndex = 0;
        if (msg.indexOf("n") == 2) {
          if (maxPlattenNummer != -1)
            filePointer.close();
          maxPlattenNummer++;
          if (SD.exists("PLATTE" + ((String)maxPlattenNummer) + ".CSV"))
            SD.remove("PLATTE" + ((String)maxPlattenNummer) + ".CSV");
          filePointer = SD.open("PLATTE" + ((String)maxPlattenNummer) + ".CSV", FILE_WRITE);
          nextIndex = msg.indexOf('>', 4);
          filePointer.print(msg.substring(4, nextIndex) + ";");//Platten-ID
          nextIndex = msg.indexOf('>', nextIndex) + 1;
          filePointer.print(msg.substring(nextIndex, msg.indexOf('>', nextIndex)) + ";");//Phys_Adr
          nextIndex = msg.indexOf('>', nextIndex) + 1;
          filePointer.println(msg.substring(nextIndex, msg.indexOf('<', nextIndex)) + ";");//RefPCB Phys
        } else {
          msg.replace("k>", "");
          filePointer.print(msg);//k>Compressed_PCB> Threshold<
        }
      }
      send("n");
      break;
    case 'n':
      state = 0;
      filePointer.close();
      setJobStatus(nextJob);
      break;
    default:
      sendError("Keine Funktion für das Kommando: " + msg);
      break;
  }
}
void send(String msg) {
  socket.flush();
  socket.println(msg);
}
void sendStatus() {
  switch (job) {
    case 0:
      send("status<br>Initialisierung");
      break;
    case 1:
      send("status<br>Bereit");
      break;
    case 2:
      send("status<br>Versuch");
      break;
    case 3:
      send("status<br>Sondermessung");
      break;
    case 4:
      send("status<br>Liest Daten");
      break;
    default:
      sendError("Job:" + ((String)job));
      break;
  }
}
void sendError(String msg) {
  send("error<br>" + msg);
}

void  sendPCBToServer(int plate, byte pcb[], int value, int threshold) {
  send("event<br>" + ((String)plate) + "<br>" + ((String)pcb[0]) + "<br>" + ((String)pcb[1]) + "<br>"
       + ((String)pcb[3]) + "<br>" + ((String)value) + "<br>" + ((String)threshold)
       + "<br>" + ((String)temp));
}
//###################################################################################################################
//################################################## Konfiguration ##################################################
//###################################################################################################################
void setupPin() {
  pinMode(22, OUTPUT);   // Messsignal Zeilen (PULLUP-PULLDOWN)
  pinMode(23, OUTPUT);   // Messsignal Spalten
  pinMode(24, OUTPUT);   // enable MUX Spalten 1 LOW
  pinMode(25, OUTPUT);   // enable MUX Spalten 1 HIGH
  pinMode(26, OUTPUT);   // S0 Spalten
  pinMode(27, OUTPUT);   // S1 Spalten
  pinMode(28, OUTPUT);   // S2 Spalten
  pinMode(29, OUTPUT);   // S3 Spalten
  pinMode(30, OUTPUT);   // enable MUX Spalten 2 LOW
  pinMode(31, OUTPUT);   // enable MUX Spalten 2 HIGH
  pinMode(32, OUTPUT);   // A_Zeile
  pinMode(33, OUTPUT);   // B_Zeile
  pinMode(34, OUTPUT);   // C_Zeile
  pinMode(35, OUTPUT);   // D_Zeile
  pinMode(36, OUTPUT);   // ungerade_Zeile
  pinMode(37, OUTPUT);   // gerade_Zeile
  pinMode(A8, INPUT);    // Messsignal Zeilen (J2,Kaskade)
  pinMode(38, OUTPUT);   // Enable_LOW_Kaskade
  pinMode(39, OUTPUT);   // ENABLE_HIGH_Kaskade
  pinMode(40, OUTPUT);   // A_Kaskade
  pinMode(41, OUTPUT);   // B_Kaskade
  pinMode(42, OUTPUT);   // C_Kaskade
  pinMode(43, OUTPUT);   // D_Kaskade
  pinMode(44, OUTPUT);   // B_Messsignal
  pinMode(45, OUTPUT);   // A_Messsignal
  //Pre-set
  digitalWrite(22, HIGH);   // Messsignal Zeilen (PULLUP)
  digitalWrite(23, LOW);   // Messsignal standartmäßig auf LOW
  digitalWrite(24, HIGH);  // Alles Adressen auf LOW und Enables auf inaktiv
  digitalWrite(25, HIGH);
  digitalWrite(26, LOW);
  digitalWrite(27, LOW);
  digitalWrite(28, LOW);
  digitalWrite(29, LOW);
  digitalWrite(30, HIGH);
  digitalWrite(31, HIGH);
  digitalWrite(32, LOW);
  digitalWrite(33, LOW);
  digitalWrite(34, LOW);
  digitalWrite(35, LOW);
  digitalWrite(36, HIGH);
  digitalWrite(37, HIGH);
  digitalWrite(38, HIGH); //
  digitalWrite(39, HIGH); //
  digitalWrite(40, LOW); //
  digitalWrite(41, LOW); //
  digitalWrite(42, LOW); //
  digitalWrite(43, LOW); //
  messsignal(0);        // Messsignal auf 1. Signal legen -> Pin 23 (LOW)
}
void messsignal(int a) { // legt das entsprechende Messsignal auf die Zeilen (Standartmessung mit Masse(LOW))
  if (a > 3 or a < 0) {
    sendError("Ungueltiges Messsignal!");
  }
  else {                   // Adressen anlegen und dann true returnen
    digitalWrite(44, a & 2); // B Adresse
    digitalWrite(45, a & 1); // A Adresse
    /*
      0 -> DigitalAusgang 23 (HIGH oder LOW setzen nicht vergessen!)
      1 -> 3,3V
      2 -> PWM
      3 -> nc
    */
  }
}
bool loadConfig() {
  if (!SD.begin(4))
    return false;
  if (!SD.exists("config.csv")) {
    return false;
  }
  filePointer = SD.open("config.csv", FILE_READ);
  String buff = filePointer.readString();
  int nextIndex = buff.indexOf(';');
  name = buff.substring(0, nextIndex);
  nextIndex++;
  convertStringToByteArray(buff.substring(nextIndex, buff.indexOf(';', nextIndex)), true);
  for (int i = 0; i < 6; i++) {
    mac[i] = castResultLong[i];
  }
  nextIndex = buff.indexOf(';', nextIndex) + 1;
  convertStringToByteArray(buff.substring(nextIndex, buff.indexOf(';', nextIndex)), false);
  for (int i = 0; i < 4; i++) {
    ip[i] = castResultShort[i];
  }
  nextIndex = buff.indexOf(';', nextIndex) + 1;
  convertStringToByteArray(buff.substring(nextIndex, buff.indexOf(';', nextIndex)), false);
  for (int i = 0; i < 4; i++) {
    serverAdr[i] = castResultShort[i];
  }
  nextIndex = buff.indexOf(';', nextIndex) + 1;
  port = buff.substring(nextIndex, buff.indexOf(';', nextIndex)).toInt();
  nextIndex = buff.indexOf(';', nextIndex) + 1;
  refNormValueMin = buff.substring(nextIndex, buff.indexOf(';', nextIndex)).toInt();
  nextIndex = buff.indexOf(';', nextIndex) + 1;
  refNormValueMax = buff.substring(nextIndex, buff.indexOf(';', nextIndex)).toInt();
  filePointer.close();
  return true;
}
//###################################################################################################################
//################################################ Hilfs-Funktionen #################################################
//###################################################################################################################
void convertStringToByteArray(String s, bool is6) {
  int nextIndex = s.indexOf('-');
  if (is6)
    castResultLong[0] = stringHexToHex(s.substring(0, nextIndex));
  else
    castResultLong[0] = (byte)s.substring(0, nextIndex).toInt();
  castResultShort[0] = castResultLong[0];
  nextIndex++;
  int ammount = 0;
  if (is6)
    ammount = 6;
  else
    ammount = 4;
  for (int i = 1; i < ammount; i++) {
    if (i < (ammount - 1)) {
      if (is6)
        castResultLong[i] = stringHexToHex(s.substring(nextIndex, s.indexOf('-', nextIndex)));
      else
        castResultLong[i] = (byte)s.substring(nextIndex, s.indexOf('-', nextIndex)).toInt();
      nextIndex = s.indexOf('-', nextIndex) + 1;
      if (i < 4)
        castResultShort[i] = castResultLong[i];
    } else {
      s.remove(0, nextIndex);
      if (is6)
        castResultLong[i] = stringHexToHex(s);
      else
        castResultLong[i] = (byte)s.toInt();
      if (i < 4)
        castResultShort[i] = castResultLong[i];
    }
  }
}
byte stringHexToHex(String s) {
  byte puffer;
  for (int i = 1; i >= 0; i--) {
    char c = s.charAt(i);
    byte tmp = 0;
    if (c >= '0' && c <= '9')
      tmp = (byte)(c - '0');
    else
      tmp = (byte)(c - 'A' + 10);
    puffer = puffer + (tmp * (pow(16, (1 - i))));
  }
  return puffer;
}
void setJobStatus(byte nJob) {
  job = nJob;
  plattenNummer = -1;
  sendStatusCheck();
  //Serial.println("Set-Job:" + ((String)job));
  newJob = 't';
}
//###################################################################################################################
//################################################### Messobjekt ####################################################
//###################################################################################################################
class Plate {
  public:
    unsigned int id;
    unsigned int maxPCB;
    unsigned int refPCB;
    unsigned int pcbInfos[1024];
    unsigned int threshold[1024];
    byte phys_adr;
    byte changes;

    void setPCB(int i, int r, int c, int m, int ec) {
      unsigned int temp = (r) << 12;
      temp = temp | ((c) << 6);
      temp = temp | (m << 4);
      temp = temp | ec;
      pcbInfos[i] = temp;
    }

    void getPCB(byte *pcb, int i) {
      pcb[0] = (pcbInfos[i] >> 12);//Row
      pcb[1] = (pcbInfos[i] >> 6 & 63);//Column
      pcb[2] = pcbInfos[i] >> 4 & 3; //Mode
      pcb[3] = pcbInfos[i] & 15; //Error Count
    }

    boolean measureRef() {// Gemessen 741
      int result = measureComponent(refPCB, 0, 7, 0);
      if ((refNormValueMin <= result) && (result <= refNormValueMax))
        return true;
      else
        return false;
    }

    void measure(int i, boolean isVersuch) {
      byte pcb[4];
      getPCB(pcb, i);
      if (isVersuch ) {
        if (pcb[3] <= maxErr) {
          int result = measureComponent(phys_adr, pcb[0], pcb[1], pcb[2]);
          if (result >= threshold[i]) {
            int result2 = -99;
            while (1) {
              delay(1);
              result2 = measureComponent(phys_adr, pcb[0], pcb[1], pcb[2]);
              if (result == result2)
                break;
              else
                result = result2;
            }
            result = result2;
            if (result >= threshold[i]) {
              if (measureRef()) {
                changes = 1;
                sendPCBToServer(id, pcb, result, threshold[i]);
                pcb[3]++;
                threshold[i] = newThreshold(i, pcb[3]);
                setPCB(i, pcb[0], pcb[1], pcb[2], pcb[3]);
              } else {
                job = 1;
                sendError("Referenzmessung ist außerhalb des Wertebereiches!");
                return;
              }
            }
          }
        }
      } else {
        int result = measureComponent(phys_adr, pcb[0], pcb[1], pcb[2]);
        sendPCBToServer(id, pcb, result, threshold[i]);
      }
    }

    int measureComponent(int e, byte r, byte c, byte dir) {
      int result = 0;
      if (((e % 2) == 0)) {   // gerade Steckplatz
        digitalWrite(37, LOW); // normal
        digitalWrite(36, HIGH); // enable MUX gerade
      }
      else {                   // ungerade Steckplatz
        digitalWrite(37, HIGH);  // Enable Mux ungerade
        digitalWrite(36, LOW);  // normal
      }
      e = zuordnungLogPhys[e - 1];
      if (e < 16) {
        digitalWrite(38, HIGH);
        digitalWrite(39, LOW); // getauscht
      } else {
        digitalWrite(38, LOW);
        digitalWrite(39, HIGH);  // getauscht
      }
      digitalWrite(40, e & 1); // Adressen fuer Kaskade
      digitalWrite(41, e & 2); //
      digitalWrite(42, e & 4); //
      digitalWrite(43, e & 8); //
      delay(1);
      digitalWrite(32, r & 1); // Adressen fuer Zeilen
      digitalWrite(33, r & 2); //
      digitalWrite(34, r & 4); //
      digitalWrite(35, r & 8); //
      if (c < 32) {               // Ansprechen MUX 1
        if (c < 16) {             // Enable MUX 1 LOW
          digitalWrite(24, HIGH); //
          digitalWrite(25, LOW);
        }
        else  {                   // Enable MUX 1 HIGH
          digitalWrite(24, LOW);
          digitalWrite(25, HIGH);
        }
        // Blockieren des MUX 2
        digitalWrite(30, HIGH);
        digitalWrite(31, HIGH);
      } else {                    // Ansprechen MUX 2
        if (c < 48) {            // Enable MUX 2 LOW
          digitalWrite(30, HIGH);
          digitalWrite(31, LOW);
        }
        else {                    // Enable MUX 2 HIGH
          digitalWrite(30, LOW);
          digitalWrite(31, HIGH);
        }
        //Blockieren des MUX 1
        digitalWrite(24, HIGH);
        digitalWrite(25, HIGH);
      }
      digitalWrite(26, c & 1);   // Adressen fuer Spalten
      digitalWrite(27, c & 2);   //
      digitalWrite(28, c & 4);   //
      digitalWrite(29, c & 8);   //
      if (dir == 0) {
        digitalWrite(22, LOW);   // Zeilen (H)
        digitalWrite(23, HIGH);  // Messsignal auf LOW (Pulldown) bei Unterbrechung
      } else if (dir == 1) {
        digitalWrite(22, HIGH);  // Messsignal Zeilen (PULLUP)
        digitalWrite(23, LOW);   // Messsignal standartmäßig auf LOW
      }
      result = ReadAnalog(8);
      return result;
    }

    int ReadAnalog(int channel) {
      int value = 0, minV = 0, maxV = 0, tmp = 0;
      for (int i = 0; i < 10; i++) {
        tmp = analogRead(channel);
        if (i == 0) {
          maxV = tmp;
          minV = tmp;
        } else {
          if (maxV < tmp)
            maxV = tmp;
          if (minV > tmp)
            minV = tmp;
        }
        value += tmp;
      }
      value -= (minV + maxV);
      return (value / 8);
    }

    int newThreshold(byte p, byte c) {
      float t = threshold[p];
      t = t + ((c) * ((1023 - threshold[p]) / (maxErr + 1)));
      if (t > 1023)    t = 1023;
      return t;
    }

    void loadPlate() {
      if (plattenNummer != -1 || (changes == 1)) {
        SD.remove("PLATTE" + ((String)plattenNummer) + ".CSV");
        filePointer = SD.open("PLATTE" + ((String)plattenNummer) + ".CSV", FILE_WRITE);
        filePointer.println(((String)id) + ";" + ((String)phys_adr) + ";" + ((String)refPCB) + ";");
        for (int i = 0; i < maxPCB; i++)
          filePointer.println(((String)pcbInfos[i]) + ";" + ((String)threshold[i]) + ";");
        filePointer.close();
      }
      changes = 0;
      plattenNummer++;
      if (plattenNummer > maxPlattenNummer)
        plattenNummer = 0;
      //Serial.print("Start reading plate" + ((String)plattenNummer) + "/" + ((String)maxPlattenNummer) + " ");
      filePointer = SD.open("PLATTE" + ((String)plattenNummer) + ".CSV", FILE_READ);
      buff = "";
      maxPCB = -1;
      int nextIndex = 0;
      while (filePointer.available()) {
        buff = filePointer.readStringUntil('\n');
        if (maxPCB == -1) { //Lese Header
          nextIndex = buff.indexOf(';');
          id = buff.substring(0, nextIndex).toInt();
          //Serial.print("ID:" + ((String)id));
          nextIndex = buff.indexOf(';', nextIndex) + 1;
          phys_adr = (byte)buff.substring(nextIndex, buff.indexOf(';', nextIndex)).toInt();
          //Serial.print(" Phy-Adr.:" + ((String)phys_adr));
          nextIndex = buff.indexOf(';', nextIndex) + 1;
          refPCB = buff.substring(nextIndex, buff.indexOf(';', nextIndex)).toInt();
          nextIndex = buff.indexOf(';', nextIndex) + 1;
          //Serial.println(" RefPCB:" + ((String)refPCB));
        } else { //Lese PCB
          nextIndex = 0;
          pcbInfos[maxPCB] = buff.substring(nextIndex, buff.indexOf(';', nextIndex)).toInt();
          nextIndex = buff.indexOf(';', nextIndex) + 1;
          threshold[maxPCB] = buff.substring(nextIndex, buff.indexOf(';', nextIndex)).toInt();
        }
        maxPCB++;
        jobStatusAction.check();
      }
      //Serial.print("width " + ((String)maxPCB) + " PCBs");
      filePointer.close();
    }
};
Plate plate;
//###################################################################################################################
//################################################### Messungen #####################################################
//###################################################################################################################
void measureTemp() {//Wird jede Sekunde aufgerufen
  uint32_t c = tc.temperatureRead();
  if (c & 0x00010000) {
    if (c & 0x00000001)
      sendError("TC [Open Circuit]");
    else if (c & 0x00000002)
      sendError("TC [Short to GND]");
    else if (c & 0x00000004)
      sendError("TC [Short to VCC]");
  } else {
    float in_f;
    float ex_f;
    unsigned char in_s = !!(c & 0x00008000);
    unsigned char ex_s = !!(c & 0x80000000);
    uint16_t ext = (c >> 16 );
    ext &= (0x7FFF);
    ext >>= 2;
    ex_f = (ext >> 2) + (ext & 0x03) * 0.25;
    if (ex_s)
      ex_f = -2048 + ex_f;
    uint16_t in = c & 0x0000FFFF;
    in >>= 4;
    in_f = (in >> 4) + (in & 0x0F) * 0.0625;
    if (in_s)
      in_s = -128 + in_s;
    temp = (int)ex_f;
    if (oldTemp == 555.55) {
      oldTemp = temp;
      if (temp >= 100) {
        startTempTrend = '1';
        startTemp = 100;
        tcDone = 't';
      } else {
        startTempTrend = '0';
        startTemp = 0;
        tcDone = 't';
      }
    }
    if (oldTemp + 3 < temp) {
      tempTrend = '1';
      oldTemp = temp;
    } else if (oldTemp - 3 > temp) {
      tempTrend = '0';
      oldTemp = temp;
    }
    if (temp >= 100) {
      if (startTemp == 0)
        tcDone = 'f';
      else if (tcDone == 'f' && startTempTrend == tempTrend) {
        send("twinc");
        tcDone = 't';
      }
    } else if (temp <= 0) {
      if (startTemp == 100)
        tcDone = 'f';
      else if (tcDone == 'f' && startTempTrend == tempTrend) {
        send("twinc");
        tcDone = 't';
      }
    }
  }
}

void measurementMode(boolean isV) {
  if (routine() == 't')
    return;
  plate.loadPlate();
  for (int i = 0; i < plate.maxPCB; i++) {
    plate.measure(i, isV);
    if (job == 1)
      return;
    if (((i % 3) == 0) && (routine() == 't'))
      return;
  }
}


