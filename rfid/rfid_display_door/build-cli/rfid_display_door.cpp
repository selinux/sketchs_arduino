#include <Arduino.h>
/**
* RFID Access Control Single
*
* This project implements a single stand-alone RFID access control
* system that can operate independently of a host computer or any
* other device. It uses an RDM630 RFID reader module to
* scan for 125KHz RFID tags, and when a recognised tag is identified
* it toggles an output for a configurable duration, typically 2
* seconds. The output can then be used to control anything.
*
* Some of this code was copy from Jonathan Oxer <jon@oxer.com.au>
* http://www.practicalarduino.com/projects/medium/rfid-access-control
*/

#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
//
// Set up the serial connection to the RFID reader module. In order to
// keep the Arduino TX and RX pins free for communication with a host,
// the sketch uses the SoftwareSerial library to implement serial
// communications on other pins.

#define XSTEP_PIN 2
#define XDIR_PIN 3
#define YSTEP_PIN 6
#define YDIR_PIN 7

#define XMS1 4
#define XMS2 5
#define YMS1 8
#define YMS2 9

#define rxPin 10
#define txPin 11
const int DISPLAY_TIME = 1000; // In milliseconds

// Create a software serial object for the connection to the RFID module
// and to the LCD display
// 12=rs(4), 11=E(6), 5=DB4(11), 4=DB5(12), 3=DB6(13), 2=DB7(14)
// 1, 5 et 16 Grd, 2 5V, 3 pot1, 4 rs, 6 E, [7-10]none, 15 pot2, 16 Gnd 
SoftwareSerial rfid = SoftwareSerial( rxPin, txPin );
//LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Specify how long the strike plate should be held open.
#define unlockSeconds 2

// The tag database consists of two parts. The first part is an array of
// tag values with each tag taking up 5 bytes. The second is a list of
// names with one name for each tag (ie: group of 5 bytes).
char* allowedTags[] = {
  "0100C1A6B3", // Tag 1
  "0100C216B5", // Tag 2
  "0100C201E5", // Tag 3
  "0100C20086", // Tag 4
};

// List of names to associate with the matching tag IDs
char* tagName[] = {
  "User 1", // Tag 1
  "User 2", // Tag 2
  "User 3", // Tag 3
  "User 4", // Tag 4
};

// Check the number of tags defined
int numberOfTags = sizeof(allowedTags)/sizeof(allowedTags[0]);

int incomingByte = 0; // To store incoming serial data

/**
* Setup
*/
void setup() {

  Serial.begin(9600); // Serial port for connection to host
  rfid.begin(9600); // Serial port for connection to RFID module

  Serial.println("RFID reader starting up");
  //lcd.begin(16, 2);
  pinMode(XDIR_PIN, OUTPUT);
  pinMode(XSTEP_PIN, OUTPUT);
  pinMode(YDIR_PIN, OUTPUT);
  pinMode(YSTEP_PIN, OUTPUT);
  pinMode(XMS1, OUTPUT);
  pinMode(XMS2, OUTPUT);
  pinMode(YMS1, OUTPUT);
  pinMode(YMS2, OUTPUT);

// set micro-step L,L = 1 (full) H,L=1/2 L,H=1/4 and H,H=1/8
  digitalWrite(XMS1, LOW);
  digitalWrite(XMS2, LOW);  
  digitalWrite(YMS1, LOW);
  digitalWrite(YMS2, LOW);
}


/**
* Search for a specific tag in the database
*/
int findTag( char tagValue[10] ) {
  for (int thisCard = 0; thisCard < numberOfTags; thisCard++) {
    // Check if the tag value matches this row in the tag database
    if(strcmp(tagValue, allowedTags[thisCard]) == 0)
    {
      // The row in the database starts at 0, so add 1 to the result so
      // that the card ID starts from 1 instead (0 represents "no match")
      return(thisCard + 1);
    }
  }
  // If we don't find the tag return a tag ID of 0 to show there was no match
  return(0);
}

void rotate(int steps, float speed){
  //rotate a specific number of microsteps (8 microsteps per step) - (negitive for reverse movement)
  //speed is any number from .01 -> 1 with 1 being fastest - Slower is stronger
  int dir = (steps > 0)? HIGH:LOW;
  steps = abs(steps);

  digitalWrite(XDIR_PIN,dir); 

  float usDelay = ( -speed);

  for(int i=0; i < steps; i++){
    digitalWrite(XSTEP_PIN, HIGH);
    //delay(usDelay); 
    delay(3);
    digitalWrite(XSTEP_PIN, LOW);
    //delay(usDelay);
    delay(3);
  }
} 


/**
* Fire the relay to activate the strike plate for the configured
* number of seconds.
*/
void unlock() {
  rotate(800, 3);
  delay(1000); 

  rotate(-800, 3); //reverse
  delay(10);
//  delay(unlockSeconds * 1000);

}

/**
* Loop
*/
void loop() {
  byte i = 0;
  byte val = 0;
  byte checksum = 0;
  byte bytesRead = 0;
  byte tempByte = 0;
  byte tagBytes[6]; // "Unique" tags are only 5 bytes but we need an extra byte for the checksum
  char tagValue[10];

  /*lcd.setCursor(0,0);
  lcd.print("Ready to read   ");
  lcd.setCursor(0,1);
  lcd.print("a RFID card     ");
  */
  // Read from the RFID module. Because this connection uses SoftwareSerial
  // there is no equivalent to the Serial.available() function, so at this
  // point the program blocks while waiting for a value from the module
  if((val = rfid.read()) == 2) { // Check for header
    bytesRead = 0;
    while (bytesRead < 12) { // Read 10 digit code + 2 digit checksum
      val = rfid.read();

      // Append the first 10 bytes (0 to 9) to the raw tag value
      if (bytesRead < 10)
      {
        tagValue[bytesRead] = val;
      }

      // Check if this is a header or stop byte before the 10 digit reading is complete
      if((val == 0x0D)||(val == 0x0A)||(val == 0x03)||(val == 0x02)) {
        break; // Stop reading
      }

      // Ascii/Hex conversion:
      if ((val >= '0') && (val <= '9')) {
        val = val - '0';
      }
      else if ((val >= 'A') && (val <= 'F')) {
        val = 10 + val - 'A';
      }

      // Every two hex-digits, add a byte to the code:
      if (bytesRead & 1 == 1) {
        // Make space for this hex-digit by shifting the previous digit 4 bits to the left
        tagBytes[bytesRead >> 1] = (val | (tempByte << 4));

        if (bytesRead >> 1 != 5) { // If we're at the checksum byte,
          checksum ^= tagBytes[bytesRead >> 1]; // Calculate the checksum... (XOR)
        };
      } else {
        tempByte = val; // Store the first hex digit first
      };

      bytesRead++; // Ready to read next digit
    }

    // Send the result to the host connected via USB
    if (bytesRead == 12) { // 12 digit read is complete
      tagValue[10] = '\0'; // Null-terminate the string

      Serial.print("Tag read: ");
      for (i=0; i<5; i++) {
        // Add a leading 0 to pad out values below 16
        if (tagBytes[i] < 16) {
          Serial.print("0");
        }
        Serial.print(tagBytes[i], HEX);
      }
      Serial.println();

      Serial.print("Checksum: ");
      Serial.print(tagBytes[5], HEX);
      Serial.println(tagBytes[5] == checksum ? " -- passed." : " -- error.");

      // Show the raw tag value
      //Serial.print("VALUE: ");
      //Serial.println(tagValue);

      // Search the tag database for this particular tag
      int tagId = findTag( tagValue );

      // Only fire the strike plate if this tag was found in the database
      if( tagId > 0 )
      {
        Serial.print("Authorized tag ID ");
       /* lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Welcome ");
        lcd.print(tagName[tagId - 1]);
	*/
        Serial.print(tagId);
        Serial.print(": unlocking for ");
        Serial.println(tagName[tagId - 1]); // Get the name for this tag from the database
        /* lcd.setCursor(0,1);
        lcd.print("door unlocked");
	*/
        unlock(); // Fire the strike plate to open the lock
      } else {
        Serial.println("Tag not authorized");
        /* lcd.setCursor(0,0);
        lcd.print("This card is not ");
        lcd.setCursor(0,1);
        lcd.print("an autorised one");
	*/
        delay(1000);
      }
      Serial.println(); // Blank separator line in output
    }

    bytesRead = 0;
  }
}
