// Sweep
// by BARRAGAN

#include <Servo.h>  //inclure le code servo
Servo myservo;      // creation de l'objet servo

int pos = 0;        // initialisation de la pos du servo
int pauseDelay = 15;
int ledPin =  2;

void setup()
{
  myservo.attach(9); 
  pinMode(ledPin, OUTPUT);
}

void loop()
{
  for(pos = 0; pos < 180; pos += 1)
 {
   myservo.write(pos);
   delay(pauseDelay);
 }
 
 for(int i = 0; i < 10; i++)
 {
   digitalWrite(ledPin, HIGH);   // set the LED on
   delay(80);                   // wait for a second
   digitalWrite(ledPin, LOW);    // set the LED off
   delay(80);
 }
  
 for(pos = 180;pos >= 1; pos -= 1);
 {
   myservo.write(pos);
   delay(pauseDelay);
 }

}
