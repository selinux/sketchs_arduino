#include <Arduino.h>
char foo;
/*------------------------------------------------------------------
-------------------------------------------------------------------*/

#ifndef Arduino_h
	#include <Arduino.h>
#endif


#include <SoftwareSerial.h>
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

#define rxPin 6
#define txPin 7
#define ARR_LEN 8
#define TAG_LEN 12

const int DISPLAY_TIME = 100; // In milliseconds

char code[20];
int val = 0;
int bytesread = 0;
String master_tag = "0100C1A6B3@5";
String target_tag[ARR_LEN] = { "0100C216B500" , "0100C201E567", "0100C20086K5" };//, "0100C18E89CL" };

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
  lcd.begin(16, 2);  
}


void loop(){
  lcd.setCursor(0,0);
  lcd.print("I'm waiting for");
  lcd.setCursor(0,1);
  lcd.print("a card to read");
  

  val = 0;
  bytesread = 0;
  int readyToRead = 0;
  boolean valid_card = false;
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

  String readed_tag = code;    // Maintenant on a une chaine de caractère plus facile à tester
  delay(3000);
  
/*========================================
  Si un RFID tag est détécté, on le teste
  ========================================*/
  
  if(bytesread >= 12 && readyToRead == 12){
      // if 12 digit read is complete
      Serial.print("Tag: [");
      for(int i=0; code[i]!='\0' ; i++)
        {
          Serial.print(code[i]);
        }
      Serial.println("]"); //print the whole 13 bytes
      Serial.println(readed_tag);
  }
  
  if(readed_tag == master_tag){
      //    Valid_card = true;
      Serial.println("ok master Tag reconnu -------------!!!!");
      lcd.clear();
      lcd.print("MASTER CARD !!!!");
      lcd.setCursor(0,1);
      lcd.print(readed_tag);
      delay(1000);
    }
  
  for( int i = 0 ; i < ARR_LEN ; i++ ){
      if( readed_tag == target_tag[i] ){
         //     Valid_card = true;
        lcd.clear();
        lcd.print("card ID ok");
        lcd.setCursor(0,1);
        lcd.print(readed_tag);
        delay( 1000 );
        break;
      }
//  if(valid_card != true ) {
//      lcd.clear();
//      lcd.print("AU VOLEUR!!!");
//      lcd.setCursor(0,1);
//      lcd.print("LA CARTE EST FAUSSE!!");
//      delay( 1000 );
//    }
  }
  delay(100);
  lcd.clear();


}


