/*     ———————————————————
*     |  Arduino Experimentation Kit Example Code             |
*     |  CIRC-02 .: 8 LED Fun :. (Multiple LEDs)   |
*     ———————————————————
*  
*  A few Simple LED animations
*
* For more information on this circuit http://tinyurl.com/d2hrud
*
*/
int ledPin =  13;    // LED connected to digital pin 13
 
// The setup() method runs once, when the sketch starts
 
void setup()   {                
  // initialize the digital pin as an output:
  pinMode(ledPin, OUTPUT);    
}
 
// the loop() method runs over and over again,
// as long as the Arduino has power
 
void loop()                    
{
  digitalWrite(ledPin, HIGH);   // set the LED on
  delay(70);                  // wait for a second
  digitalWrite(ledPin, LOW);    // set the LED off
  delay(70);                  // wait for a second
}