#include <Arduino.h>
/*
  Door control motor test program
 */
 boolean motorDir = true;
 unsigned int stepDelay = 2000;
 unsigned int stepNumber = 100;
 

void help()
{
  Serial.println("Door Motor Test Program.");
  Serial.println("Enter h to show this help text");
  Serial.println("Enter s to show a summary of all settings");
  Serial.println("Enter d to invert the motor direction");
  Serial.println("Enter t to set the step delay time");
  Serial.println("Enter n to set the number of steps");
  Serial.println("Enter x to execute the motor movement");
  Serial.println("-------------------------------------");
}

void summary()
{
  Serial.print("Motor direction is ");
  if (motorDir == true)
  {
    Serial.println("clockwise");
  }
  else
  {
    Serial.println("counter-clockwise");
  }
  Serial.print("Step delay is ");
  Serial.print(stepDelay);
  Serial.println(" us");
  Serial.print("Step Number is ");
  Serial.println(stepNumber);
  Serial.println("-------------------------------------");
}

void invertMotorDir()
{
  Serial.print("Setting motor direction to ");
  if (motorDir == false)
  {
    motorDir=true;
    Serial.println("clockwise");
  }
  else
  {
    motorDir=false;
    Serial.println("counter-clockwise");
  }
  Serial.println("-------------------------------------");
}

void SetStepDelay()
{
  Serial.println("Enter step delay in us:");
  while(!(Serial.available()))
  {
    //wait until number entered
  }
  stepDelay = Serial.parseInt();
  Serial.print("Step Delay set to ");
  Serial.print(stepDelay);
  Serial.println(" us");
  Serial.println("-------------------------------------");
}

void SetStepNumber()
{
  Serial.println("Enter step number");
  while(!(Serial.available()))
  {
    //wait until number entered
  }
  stepNumber = Serial.parseInt();
  Serial.print("Step Number set to ");
  Serial.println(stepNumber);
  Serial.println("-------------------------------------");
}
void execMovement()
{
  digitalWrite(11, HIGH); //wake up from sleep
  if (motorDir == true)
  {
    digitalWrite(3, HIGH);
  }
  else
  {
    digitalWrite(3, LOW);
  }
  
  Serial.println("Starting to turn motor...");
  for (int n=0; n<stepNumber; n++)
  {
    digitalWrite(2, HIGH);   // set the LED on
    delayMicroseconds(stepDelay/2);              // wait for a second
    digitalWrite(2, LOW);    // set the LED off
    delayMicroseconds(stepDelay/2);             // wait for a second
  }
  digitalWrite(11, LOW); //go back to sleep sleep
  Serial.println("Movement finished");
  Serial.println("-------------------------------------");
}

 void setup()
{
  // start serial port at 9600 bps:
  pinMode(2, OUTPUT);          //  motor step 
  pinMode(3, OUTPUT);	       //  motor direction
  pinMode(11, OUTPUT);         // not used
  Serial.begin(9600);
  help();
  
}

void loop()
{
  char command;
  
  if (Serial.available() > 0) {
    // read the incoming byte:
    command = Serial.read();
    // say what you got:
    //Serial.print("I received: ");
    //Serial.println(command);
    switch (command) {
      case 'h': //help
      help();
      break;
      case 's': //settings summary
      summary();
      break;
      case 'd': //invert motor direction
      invertMotorDir();
      break;
      case 't': //set step delay time
      SetStepDelay();
      break;
      case 'n': //set number of steps
      SetStepNumber();
      break;
      case 'x': //execute motor movement
      execMovement();
      break;
      default:
      Serial.println("Unrecognized command");
    }
  }
}
