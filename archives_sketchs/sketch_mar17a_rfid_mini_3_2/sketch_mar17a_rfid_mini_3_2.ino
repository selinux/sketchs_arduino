/*------------------------------------------------------------------
This is a sample code for RDM630 RFID reader by Spekel(Spekel.se)
This work is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License.
http://creativecommons.org/licenses/by-nc-sa/3.0/
-------------------------------------------------------------------*/
#include <SoftwareSerial.h>
#define rxPin 2
#define txPin 3

#define TAG_LEN 12
#define ARR_LEN 8

const int DISPLAY_TIME = 100; // In milliseconds

const int switchPin = 8;
const int redLedPin = 5;
const int greenLedPin = 6;
const int blueLedPin = 7;
const int buzzerPin = 12;

int red = 0;
int green = 1;
int blue = 2;

//int tone = 2550;

// Used to store the current intensity level of the individual LEDs
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


void alarm () {
  if (digitalRead(switchPin) == 1) {
    for (long i = 0 ; i < 70000L ; i += 3038 ) {
      digitalWrite(buzzerPin , HIGH);
      delayMicroseconds(1519);
      digitalWrite(buzzerPin , LOW);
      delayMicroseconds(1519);
    } 
  }
}



void setup()
{
  Serial.begin(9600);
  Serial.println("Serial Ready");
  RFID.begin(9600);
  Serial.println("RFID Ready");
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(switchPin, INPUT);  //pinMode(ledPin, OUTPUT);
}


void loop()
{
  val = 0;
  bytesread = 0;
  int readyToRead = 0;
  boolean validCard = false;
//  String readedTag = "";
  
  
/*========================================
  read 12 digit code
  ========================================*/
  
  while(bytesread < 12)
  {
      val = RFID.read();
      if(val == 3)
          { // if header or stop bytes before the 10 digit reading
            break; // stop reading
          }
          
      if(val != 2)
          {
            code[bytesread] = val; // add the digit
            bytesread++; // ready to read next digit
            code[bytesread] = '\0'; // add the NULL
            if(val != -1)
             {
               readyToRead++;
             }
          }
  }
// Debug Serial.println(readyToRead);

String readedTag = code;    // Maintenant on a une chaine de caractère plus facile à tester

/*========================================
  Si un RFID tag est détécté, on le teste
  ========================================*/
  
  if(bytesread >= 12 && readyToRead == 12) {                // if 12 digit read is complete
    
    Serial.print("Tag: [");
    
    for(int i=0; code[i]!='\0' ; i++) {
          Serial.print(code[i]); 
    }

    Serial.println("]"); //print the whole 13 bytes
    Serial.println(readedTag);
  

  
  if(readedTag == masterTag) {

    Serial.println("ok master Tag reconnu -------------!!!!");
    signal(blue);
    validCard = true;
    
  }
  
  for( int i = 0 ; i < ARR_LEN ; i++ ) {
    if( readedTag == targetTag[i] ) {
      signal(green);
      validCard = true;
      delay( 5000 );
      break;
    } 
  }
  if(validCard != true ) {
      signal(red);
    }
  }
 alarm();

}

void signal (int color) {
  
  int colorLed[3] = { redLedPin , greenLedPin , blueLedPin };
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



