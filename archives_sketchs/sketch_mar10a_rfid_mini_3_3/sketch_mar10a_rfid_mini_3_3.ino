/*------------------------------------------------------------------
This is a sample code for RDM630 RFID reader by Spekel(Spekel.se)
This work is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License.
http://creativecommons.org/licenses/by-nc-sa/3.0/
-------------------------------------------------------------------*/
#include <SoftwareSerial.h>

#define rxPin 2
#define txPin 3
#define ARR_LEN 8

const int DISPLAY_TIME = 100; // In milliseconds

const int switchPin = 8;
const int redLedPin = 9;
const int greenLedPin = 10;
const int blueLedPin = 11;
const int buzzerPin = 12;

int red = 0;
int green = 1;
int blue = 2;


int redIntensity = 0;
int greenIntensity = 0;
int blueIntensity = 0;

char code[20];
int val = 0;
int bytesread = 0;
String masterTag = "0100C1A6B3@5";
String targetTag[ARR_LEN] = { "0100C216B500" , "0100C201E567" } ;//, "0100C20086K5" , "0100C18E89CL" };

//------------------------------------
//create a Serial object RFID
SoftwareSerial RFID= SoftwareSerial(rxPin, txPin);






void setup(){
  Serial.begin(9600);
  Serial.println("Serial Ready");
  RFID.begin(9600);
  Serial.println("RFID Ready");
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(switchPin, INPUT);  //pinMode(ledPin, OUTPUT);
}


void loop(){
 
  delay(2000);
    
  if(digitalRead(switchPin) == 1){

    do{
      delay(1000);
      String readedTag = readCard();   //    read 12 digit code
      if(readedTag != "none" ){                // if 12 digit read is complete
	boolean stateTag = testTag(readedTag);
	if(stateTag){
	  Serial.println("l'alarm est deconnéctée pendant 5s");
	  //delay(5000);
	  break;
	}
      }
 
      Serial.println("la porte est sous alarm ------------!");
    } while(digitalRead(switchPin) == 1);
    
    if(digitalRead(switchPin) == 0){
      alarm();
    }
  } else {
    Serial.println("la porte est ouverte ce n'est pas la peine de s'ammorcer ! ");
  }

}

boolean card(){
  
}

boolean testTag(String tag){
  boolean validCard = false;
  if(tag == masterTag){
    Serial.println("ok master Tag reconnu -------------!!!!");
    signal(blue);
    validCard = true;
  }
  
  for( int i = 0 ; i < ARR_LEN ; i++ ){
    if( tag == targetTag[i] ){
      signal(green);
      validCard = true;
      //Serial.println("ok Tag reconnu !");
      delay( 100 );
      break;
    } 
  }
  if(validCard != true ) {
    Serial.println("Tag non reconnu !");
      signal(red);
    }
  return validCard;
}


String readCard(){
  val = 0;
  bytesread = 0;
  int readyToRead = 0;

  while(bytesread < 12) {
    val = RFID.read();
    if(val == 3) { // if header or stop bytes before the 10 digit reading
      break; // stop reading
    }
    
    if(val != 2) {
      code[bytesread] = val; // add the digit
      bytesread++; // ready to read next digit
      code[bytesread] = '\0'; // add the NULL
      if(val != -1) {
	readyToRead++;
      }
    }
  }
  //debugTag();
  String tag = code;    // Maintenant on a une chaine de caractère plus facile à tester 
  if(bytesread >= 12 && readyToRead == 12){
    return tag;
  } else {
    tag = "none";
    return tag;
  }
}





void signal(int color){
  
  int colorLed[] = { redLedPin , greenLedPin , blueLedPin };
  analogWrite(colorLed[color], greenIntensity <= 255);
  if (color == 0 ) {
    for (long i = 0 ; i < 20000L ; i += 3038 ) {
      digitalWrite(buzzerPin , HIGH);
      delayMicroseconds(1519);
      digitalWrite(buzzerPin , LOW);
      delayMicroseconds(1519);
    } 
  } else {
    delay(DISPLAY_TIME);
  }
  analogWrite(colorLed[color], LOW);
  delay(DISPLAY_TIME);
  
}


void alarm(){

for (int i = 0; i < 10; ++i)
  {
    for (long j = 0 ; j < 700000L ; j += 3038 ) {
    digitalWrite(buzzerPin , HIGH);
    delayMicroseconds(1519);
    digitalWrite(buzzerPin , LOW);
    delayMicroseconds(1519);
    }
    delay(500);
    Serial.println("!!!!!!!!!!!!!---------------ALARM-----------------!!!!!!!!!!!!!!!!!");
    String readedTag = readCard();   //    read 12 digit code
    if(readedTag != "none" ){                // if 12 digit read is complete
      boolean stateTag = testTag(readedTag);
      if(stateTag){
	Serial.println("Carte détéctée.........j'arrête l'alarm !");
	break;
      }
    }
  }
}



/* void debugTag () { */
/*   Serial.print("Tag: ["); */
      
/*   for(int i=0; code[i]!='\0' ; i++) { */
/*     Serial.print(code[i]);  */
/*   } */
    
/*   Serial.println("]"); //print the whole 13 bytes */
/*   Serial.println(readedTag); */
/* } */

