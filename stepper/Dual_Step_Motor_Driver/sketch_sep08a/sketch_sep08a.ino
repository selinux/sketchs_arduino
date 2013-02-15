//////////////////////////////////////////////////////////////////
//©2011 bildr
//Released under the MIT License - Please reuse change and share
//Using the easy stepper with your arduino
//use rotate and/or rotateDeg to controll stepper motor
//speed is any number from .01 -> 1 with 1 being fastest - 
//Slower Speed == Stronger movement
/////////////////////////////////////////////////////////////////

#define XSTEP_PIN 2
#define XDIR_PIN 3
#define YSTEP_PIN 6
#define YDIR_PIN 7

#define XMS1 4
#define XMS2 5
#define YMS1 8
#define YMS2 9

 
void setup() {
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

void rotate(int steps, float speed){
  //rotate a specific number of microsteps (8 microsteps per step) - (negitive for reverse movement)
  //speed is any number from .01 -> 1 with 1 being fastest - Slower is stronger
  int dir = (steps > 0)? HIGH:LOW;
  steps = abs(steps);

  digitalWrite(XDIR_PIN,dir); 

  float usDelay = ( -speed);

  for(int i=0; i < steps; i++){
    digitalWrite(XSTEP_PIN, HIGH);
    delay(usDelay); 
    //delay(3);
    digitalWrite(XSTEP_PIN, LOW);
    delay(usDelay);
    //delay(3);
  }
} 


void loop(){ 

  //rotate a specific number of degrees 
  //rotateDeg(360, 1);
 // delay(1);

//  rotateDeg(-360, .1);  //reverse
 // delay(1); 

  //rotate a specific number of microsteps (with 8 microsteps per step)
  //a 200 step stepper would take 1600 micro steps for one full revolution
  rotate(800, 3);
  delay(10); 

  rotate(-800, 3); //reverse
  delay(10);
}


