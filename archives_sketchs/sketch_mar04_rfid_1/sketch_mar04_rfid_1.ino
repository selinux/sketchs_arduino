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
const int RED_LED_PIN = 9;
const int GREEN_LED_PIN = 10;
const int BLUE_LED_PIN = 11;

// Used to store the current intensity level of the individual LEDs
int redIntensity = 0;
int greenIntensity = 0;
int blueIntensity = 0;

char code[20];
int val = 0;
int bytesread = 0;
char master_tag[TAG_LEN] = {'0','1','0','0','C','1','8','E','8','9','C','L'};
char target_tag[ARR_LEN][TAG_LEN] = 
{{'0','1','0','0','C','2','0','1','E','5','6','7'},
// {'0','1','0','0','C','2','1','6','B','5','0','0'},
// {'0','1','0','0','C','1','A','6','B','3','@','5'},
 {'0','1','0','0','C','2','0','0','8','6','K','5'}
 };

//------------------------------------
//create a Serial object RFID
SoftwareSerial RFID= SoftwareSerial(rxPin, txPin);

void setup()
{
  Serial.begin(9600);
  Serial.println("Serial Ready");
  RFID.begin(9600);
  Serial.println("RFID Ready");
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
   //pinMode(ledPin, OUTPUT);
}


void loop()
{
  val = 0;
  bytesread = 0;
  int readyToRead = 0;

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
// Debug        
Serial.println(readyToRead);
/*========================================
  Si un RFID est détécté, on le teser
  ========================================*/
  
  if(bytesread >= 12 && readyToRead == 12)//&& val >= -1 ) // memcmp(code, '\0', 4 ) != 0)
  { // if 12 digit read is complete
      Serial.print("Tag: [");
      for(int i=0; code[i]!='\0' ; i++)
        {
          Serial.print(code[i]);
        }
      Serial.println("]"); //print the whole 13 bytes
  
      
      if(memcmp(code, master_tag, TAG_LEN) == 0)
         {
           analogWrite(BLUE_LED_PIN, greenIntensity <= 255);
           delay(DISPLAY_TIME);
           analogWrite(BLUE_LED_PIN, LOW);
           delay(DISPLAY_TIME);
         }
       for(int i = 0; i < ARR_LEN; i++)  //Serial.println(target_tag[1]);
         {
           if(memcmp(code, target_tag[i], TAG_LEN) == 0 )
              {
                 analogWrite(GREEN_LED_PIN, greenIntensity <= 255);
                 delay(DISPLAY_TIME);
                 analogWrite(GREEN_LED_PIN, LOW);
                 delay(DISPLAY_TIME);
               }
          }
          
        analogWrite(RED_LED_PIN, greenIntensity <= 255);
        delay(DISPLAY_TIME);
        analogWrite(RED_LED_PIN, LOW);
        delay(DISPLAY_TIME);
        
    
  }
     
}







