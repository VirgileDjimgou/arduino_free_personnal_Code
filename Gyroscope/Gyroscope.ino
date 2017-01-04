//Arduino 1.0+ only

#include <Wire.h>
#include <ADXL345.h>

const float alpha = 0.5;

double fXg = 0;
double fYg = 0;
double fZg = 0;

ADXL345 acc;


#define CTRL_REG1 0x20
#define CTRL_REG2 0x21
#define CTRL_REG3 0x22
#define CTRL_REG4 0x23
#define CTRL_REG5 0x24

int L3G4200D_Address = 105; //I2C adresse du composant L3G4200D (gyroscope)

int x;
int y;
int z;


#define BMP085_ADDRESS 0x77  // I2C addresse du  BMP085 (baro et thermometre)

const unsigned char OSS = 0;  // adresse de transition

// valeur Calibration
int ac1;
int ac2;
int ac3;
unsigned int ac4;
unsigned int ac5;
unsigned int ac6;
int b1;
int b2;
int mb;
int mc;
int md;
long b5; 


void setup(){

  Wire.begin();
  Serial.begin(9600);
  setupL3G4200D(2000); // Configurration L3G4200  - 250, 500 or 2000 deg/sec
  bmp085Calibration(); // configuration  barometer + pressure 
  delay(100); //wait for the sensor to be ready 
}


// loop 
void loop(){
  gyro_acce_sensor();
  gps_data();
 } 
 
 void gps_data(){
   
    Serial.print("gps");
    Serial.print(":");
    Serial.print("E 0725.89");
    Serial.print(":");
    Serial.print("S 5265.03");
    
    // fermeture de la trame gps toujours 
    Serial.println();
   }
 
 
 //debut lecture et envoie valeur  accelerometer  gyroscope, barometre et thermometre  
 
void gyro_acce_sensor(){
  
  
  float temperature = bmp085GetTemperature(bmp085ReadUT()); //doit etre appeler en premier 
  float pressure = bmp085GetPressure(bmp085ReadUP());
  float atm = pressure / 101325; // "standard atmosphere"
  float altitude = calcAltitude(pressure); //calcul incompensé en - Meters 
  
  delay(20); //delai d'attente pour initialisatisation des registres concernées
  
  getGyroValues();  //  update x, y, and z  du gyroscope
  
  delay(20); // delay d'attente ... 
  
  double pitch_acc, roll_acc, Xg, Yg, Zg;
  acc.read(&Xg, &Yg, &Zg);
  
  delay(20); // delay d'attente ... 
  //filtre passe-bas pour le traitemenet et le filtrage accelerometer
  fXg = Xg * alpha + (fXg * (1.0 - alpha));
  fYg = Yg * alpha + (fYg * (1.0 - alpha));
  fZg = Zg * alpha + (fZg * (1.0 - alpha));
  
  //Roll & Pitch Equations determinantes... peuvent  etres mieux faite ! mais bon ca fonctionne 
  // c lessentiel
  roll_acc  = (atan2(-fYg, fZg)*180.0)/M_PI;
  pitch_acc = (atan2(fXg, sqrt(fYg*fYg + fZg*fZg))*180.0)/M_PI;


// ebfin envoie des valeurs 
  Serial.print("imu:");
  Serial.print(pitch_acc);  // valeur accelerometer
  Serial.print(":");
  Serial.print(roll_acc);
  
  
  Serial.print(":");
  Serial.print(x);          // valeur gyroscope
  Serial.print(":");
  Serial.print(y);
  Serial.print(":");
  Serial.print(z);
  Serial.print(":");
  //Serial.print("Temperature: ");
  Serial.print(temperature, 2); //display 2 decimal places
  //Serial.println("deg C");
  //Serial.print("Pressure: ");
  Serial.print(":");
  Serial.print(pressure, 0); //whole number only.
  //Serial.println(" Pa");
  //Serial.print("Standard Atmosphere: ");
  Serial.print(":");
  Serial.print(atm, 4); //display 4 decimal places
  //Serial.print("Altitude: ");
  Serial.print(":");
  Serial.print(altitude, 2); //display 2 decimal places
  
  
  // pour fermer la trames ...hyper important sinon risque de crash
  Serial.println();
  
}

void getGyroValues(){

  byte xMSB = readRegister(L3G4200D_Address, 0x29);
  byte xLSB = readRegister(L3G4200D_Address, 0x28);
  x = ((xMSB << 8) | xLSB);

  byte yMSB = readRegister(L3G4200D_Address, 0x2B);
  byte yLSB = readRegister(L3G4200D_Address, 0x2A);
  y = ((yMSB << 8) | yLSB);

  byte zMSB = readRegister(L3G4200D_Address, 0x2D);
  byte zLSB = readRegister(L3G4200D_Address, 0x2C);
  z = ((zMSB << 8) | zLSB);
}

// configuration standart  de ce composant ... an e surtout plus modifier 
int setupL3G4200D(int scale){

  // initialisation x,y,z ... on doit en principe l'etaindre d'abord
  writeRegister(L3G4200D_Address, CTRL_REG1, 0b00001111);

  // ecriture  sur le registre de control reg 2 pour lajustage du hff 
  writeRegister(L3G4200D_Address, CTRL_REG2, 0b00000000);
  writeRegister(L3G4200D_Address, CTRL_REG3, 0b00001000);
  if(scale == 250){
    writeRegister(L3G4200D_Address, CTRL_REG4, 0b00000000);
  }else if(scale == 500){
    writeRegister(L3G4200D_Address, CTRL_REG4, 0b00010000);
  }else{
    writeRegister(L3G4200D_Address, CTRL_REG4, 0b00110000);
  }
  writeRegister(L3G4200D_Address, CTRL_REG5, 0b00000000);
}


/// fonction  barometre  et calibration du barometre 

void bmp085Calibration()
{
  ac1 = bmp085ReadInt(0xAA);
  ac2 = bmp085ReadInt(0xAC);
  ac3 = bmp085ReadInt(0xAE);
  ac4 = bmp085ReadInt(0xB0);
  ac5 = bmp085ReadInt(0xB2);
  ac6 = bmp085ReadInt(0xB4);
  b1 = bmp085ReadInt(0xB6);
  b2 = bmp085ReadInt(0xB8);
  mb = bmp085ReadInt(0xBA);
  mc = bmp085ReadInt(0xBC);
  md = bmp085ReadInt(0xBE);
}

// Calcul temperature en deg C
float bmp085GetTemperature(unsigned int ut){
  long x1, x2;

  x1 = (((long)ut - (long)ac6)*(long)ac5) >> 15;
  x2 = ((long)mc << 11)/(x1 + md);
  b5 = x1 + x2;

  float temp = ((b5 + 8)>>4);
  temp = temp /10;

  return temp;
}

// pression 
long bmp085GetPressure(unsigned long up){
  long x1, x2, x3, b3, b6, p;
  unsigned long b4, b7;

  b6 = b5 - 4000;
  // Calculate B3
  x1 = (b2 * (b6 * b6)>>12)>>11;
  x2 = (ac2 * b6)>>11;
  x3 = x1 + x2;
  b3 = (((((long)ac1)*4 + x3)<<OSS) + 2)>>2;

  // Calculate B4
  x1 = (ac3 * b6)>>13;
  x2 = (b1 * ((b6 * b6)>>12))>>16;
  x3 = ((x1 + x2) + 2)>>2;
  b4 = (ac4 * (unsigned long)(x3 + 32768))>>15;

  b7 = ((unsigned long)(up - b3) * (50000>>OSS));
  if (b7 < 0x80000000)
    p = (b7<<1)/b4;
  else
    p = (b7/b4)<<1;

  x1 = (p>>8) * (p>>8);
  x1 = (x1 * 3038)>>16;
  x2 = (-7357 * p)>>16;
  p += (x1 + x2 + 3791)>>4;

  long temp = p;
  return temp;
}

// debut du protocolage I2C

char bmp085Read(unsigned char address)
{
  unsigned char data;

  Wire.beginTransmission(BMP085_ADDRESS);
  Wire.write(address);
  Wire.endTransmission();

  Wire.requestFrom(BMP085_ADDRESS, 1);
  while(!Wire.available())
    ;

  return Wire.read();
}

int bmp085ReadInt(unsigned char address)
{
  unsigned char msb, lsb;

  Wire.beginTransmission(BMP085_ADDRESS);
  Wire.write(address);
  Wire.endTransmission();

  Wire.requestFrom(BMP085_ADDRESS, 2);
  while(Wire.available()<2)
    ;
  msb = Wire.read();
  lsb = Wire.read();

  return (int) msb<<8 | lsb;
}

// Read the uncompensated temperature value
unsigned int bmp085ReadUT(){
  unsigned int ut;

  // Write 0x2E into Register 0xF4
  // This requests a temperature reading
  Wire.beginTransmission(BMP085_ADDRESS);
  Wire.write(0xF4);
  Wire.write(0x2E);
  Wire.endTransmission();

  // Wait at least 4.5ms
  delay(5);

  // Read two bytes from registers 0xF6 and 0xF7
  ut = bmp085ReadInt(0xF6);
  return ut;
}

// lecture pression incompens´´e ... bon assez stable 
unsigned long bmp085ReadUP(){

  unsigned char msb, lsb, xlsb;
  unsigned long up = 0;

 
  Wire.beginTransmission(BMP085_ADDRESS);
  Wire.write(0xF4);
  Wire.write(0x34 + (OSS<<6));
  Wire.endTransmission();

  // attente  d'un delai de conversion sur le registre 085
  delay(2 + (3<<OSS));

  // lecture des registres 0xF6 (MSB), 0xF7 (LSB), and 0xF8 (XLSB)
  msb = bmp085Read(0xF6);
  lsb = bmp085Read(0xF7);
  xlsb = bmp085Read(0xF8);

  up = (((unsigned long) msb << 16) | ((unsigned long) lsb << 8) | (unsigned long) xlsb) >> (8-OSS);

  return up;
}


float calcAltitude(float pressure){

  float A = pressure/101325;
  float B = 1/5.25588;
  float C = pow(A,B);
  C = 1 - C;
  C = C /0.0000225577;

  return C;
}

// fonction non utilise ... mais on verra plus tard ce que l'on en fera 
void bar_sensor()
{
  float temperature = bmp085GetTemperature(bmp085ReadUT()); //MUST be called first
  float pressure = bmp085GetPressure(bmp085ReadUP());
  float atm = pressure / 101325; // "standard atmosphere"
  float altitude = calcAltitude(pressure); //Uncompensated caculation - in Meters 

  //Serial.print("Temperature: ");
  Serial.print(temperature, 2); //display 2 decimal places
  //Serial.println("deg C");
  //Serial.print("Pressure: ");
  Serial.print(pressure, 0); //whole number only.
  //Serial.println(" Pa");
  //Serial.print("Standard Atmosphere: ");
  Serial.print(atm, 4); //display 4 decimal places

  //Serial.print("Altitude: ");
  Serial.println(altitude, 2); //display 2 decimal places
  //Serial.println(" M");

  //Serial.println();//line break

  //delay(1000); //wait a second and get values again.
}

/// end barometer lecture 



// pure fonction  wire ... de facon generique lecture et ecriture 
/// ici suis fier de moi.... c'etait pas gagner d'avance 

void writeRegister(int deviceAddress, byte address, byte val) {
    Wire.beginTransmission(deviceAddress); // start transmission to device 
    Wire.write(address);       // send register address
    Wire.write(val);         // send value to write
    Wire.endTransmission();     // end transmission
}

int readRegister(int deviceAddress, byte address){

    int v;
    Wire.beginTransmission(deviceAddress);
    Wire.write(address); // register to read
    Wire.endTransmission();
    Wire.requestFrom(deviceAddress, 1); // read a byte
    while(!Wire.available()) {
        // waiting
    }

    v = Wire.read();
    return v;
}
