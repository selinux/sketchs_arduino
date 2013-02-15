#include <SoftwareSerial.h>


const int ledPin = 7;
const int sensorPin = 4;
int val;


void setup() {
  
  Serial.begin(9600);
  Serial.println("Serial Ready");
  pinMode(ledPin, OUTPUT);
  pinMode(sensorPin, INPUT);
}

void loop() {
  //int sensorVal;
  //int tmpVal;
  //digitalWrite(ledPin, HIGH);
 
  val = digitalRead(sensorPin);
  //tmpVal = sensorVal;
  delay(100);
  //sensorVal = analogRead(sensorPin);
  //val = map( val, 0, 600, 0, 100);
  Serial.println(val);
  if ( val == HIGH) {
   Serial.println("detecteur activé -----");
  } 
  /*if ( sensorVal >= 5 ){
    Serial.println("Ok ----------------------!!!!!!");
  }*/
  
 // Serial.println(tmpVal);

 //val = map( val, 200, 300, 0, 100);
 //Serial.println(val);
  //Serial.println(diffVal);
}
