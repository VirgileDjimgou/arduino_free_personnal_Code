
 #include <Servo.h> 
 
Servo Servo_1; 
Servo Servo_2;

String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete

void setup() {
  // initialize serial:#
  Servo_1.attach(9);
  Servo_2.attach(10);  
  Serial.begin(9600); // Baud
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
}

void loop() {
  // print the string when a newline arrives:
  if (stringComplete) { // stringComplete = true
    String Ser = inputString.substring(0,2);
    String Ser_val =inputString.substring(3,6);
    
    if(Ser=="s1"){
      Servo_1.write(Ser_val.toInt());
      delay(15);
    
    }
    if(Ser=="s2"){
    Servo_2.write(Ser_val.toInt());
    delay(15);
    }
     
     Serial.println(Ser + " = " + Ser_val.toInt());
    // Servo_1.write(inputString.toInt());
   
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
}

/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}


