/*
  Serial remote control for Nikon DSLR
*/
 unsigned int picsDelay = 10;
 unsigned int NumberOfPics = 5;



#include <nikonIrControl.h>

int CameraIrPin = 13;                // LED connected to digital pin 13

void setup()                    // run once, when the sketch starts
{
  pinMode(CameraIrPin, OUTPUT);      // sets the digital pin as output
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
      help();
      break;
      case 'd': //invert motor direction
      cameraSnap(CameraIrPin);
      Serial.println("The camera shall have triggered...");
      help();
      break;
      case 't': //set step delay time
      SetStepDelay();
      help();
      break;
      case 'n': //set number of steps
      SetStepNumber();
      help();
      break;
      case 'x': //execute motor movement
      execMovement();
      help();
      break;
      default:
      Serial.println("Unrecognized command");
    }
  }
}

void help()
{
  Serial.println("Nikon remote control program.");
  Serial.println("Enter h to show this help");
  Serial.println("Enter s to show a summary of all settings");
  Serial.println("Enter d to take a picture directly");
  Serial.println("Enter t to set the step delay (s)");
  Serial.println("Enter n to set the number of pictures");
  Serial.println("Enter x to take a picture in n secondes");
  Serial.println("-------------------------------------");
}

void summary()
{
  Serial.print("Step delay is ");
  Serial.print(picsDelay);
  Serial.println(" s");
  Serial.print("Step Number is ");
  Serial.println(NumberOfPics);
  Serial.println("-------------------------------------");
}


void SetStepDelay()
{
  Serial.println("Enter step delay in secondes :");
  while(!(Serial.available()))
  {
    //wait until number entered
  }
  picsDelay = Serial.parseInt();
  Serial.print("Step Delay set to ");
  Serial.print(picsDelay);
  Serial.println(" s");
  Serial.println("-------------------------------------");
}

void SetStepNumber()
{
  Serial.println("Enter step number");
  while(!(Serial.available()))
  {
    //wait until number entered
  }
  NumberOfPics = Serial.parseInt();
  Serial.print("Number of pics is ");
  Serial.println(NumberOfPics);
  Serial.println("-------------------------------------");
}
void execMovement()
{
  Serial.println("Starting...");
  for (int n=1; n<=NumberOfPics; n++)
  {
    delay(picsDelay*1000);
    cameraSnap(CameraIrPin);
    Serial.print("The camera shall have triggered the pics n° ");
    Serial.print(n);
    Serial.println(" ...");
  }
  Serial.println("Done...");
}
