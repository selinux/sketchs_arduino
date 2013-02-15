#include <SoftwareSerial.h>

#define rxPin 3
#define txPin 2

#define TAG_LEN 20
#define ARR_LEN 2
// set up a new serial
SoftwareSerial mySerial =  SoftwareSerial(rxPin, txPin);

int ledPin = 13;
int serIn;
byte code[TAG_LEN];		 // var that will hold the bytes-in read from the serialBuffer
int bytes_read = 0;
char target_tag[ARR_LEN][TAG_LEN] =
{{0x00, 0x00, 0xdd, 0x8e, 0x96, 0x0e, 0x00, 0x01, 0x04, 0xe0, 0x10, 0xc8 },
{0x00,  0x00, 0x1b, 0x4e, 0xc1, 0x0d, 0x00, 0x01, 0x04, 0xe0, 0xd5, 0x7b}},
{0x00,  0x00, 0x1b, 0x4e, 0xc1, 0x0d, 0x00, 0x01, 0x04, 0xe0, 0xd5, 0x7b};

void setup()  {
  // define pin modes for tx, rx, led pins:
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  mySerial.begin(19200); // set the data rate for the SoftwareSerial port
  Serial.begin(19200);	// opens serial port, sets data rate to 19200 bps
}


void loop () {

  mySerial.print("0xfa, BYTE"); //request Tag code

  if(Serial.available()){
     //keep reading from serial untill there are bytes in the serial buffer
     while (Serial.available() && bytes_read < TAG_LEN){		    //read Serial
     code[bytes_read] = Serial.read();
     bytes_read++;  // ready to read next digit
     }
  }
  //do somenthing else perhaps wait for other data.
  Serial.println ("NO TAG ");

  //print out later in the loop the sentence only if it has actually been collected;
  if( bytes_read >= TAG_LEN){
    for(int i=0; i<bytes_read; i++){
	    Serial.print(code[i], HEX);    //print out the byte at the specified index
		Serial.print(",");
    }
    for(int i = 0; i < ARR_LEN; i++)
    {
	if(memcmp(code, target_tag[i], TAG_LEN) == 0 )
	{
	    digitalWrite(ledPin, HIGH);
	    delay(500);
	    digitalWrite(ledPin, LOW);
	    delay(500);
	}

    }
    Serial.flush(); //flush serial buffer
 }

  Serial.println();
  delay(100);
  bytes_read = 0;

  }
