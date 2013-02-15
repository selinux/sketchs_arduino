char foo;
/*------------------------------------------------------------------
-------------------------------------------------------------------*/

#ifndef Arduino_h
	#include <Arduino.h>
#endif


#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
#include <OneWire.h>
#include <math.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
OneWire  ds(10);  // on pin 10


//CUSTOM LCD CHARACTERS 
   uint8_t custom_0[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; //blank
   uint8_t custom_1[8] = { 0x00, 0x16, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11}; //1line
   uint8_t custom_2[8] = { 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18}; //5line
   uint8_t custom_3[8] = { 0x00, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A}; //6line
   uint8_t custom_4[8] = { 0x00, 0x1B, 0x1B, 0x1B, 0x1B, 0x1B, 0x1B, 0x1B}; //4line
   uint8_t custom_5[8] = { 0x03, 0x1B, 0x1B, 0x1B, 0x1B, 0x1B, 0x1B, 0x1B}; //7line redundant.
   uint8_t custom_6[8] = { 0x06, 0x09, 0x09, 0x06, 0x00, 0x00, 0x00, 0x00}; // degree
   uint8_t custom_7[8] = { 0x03, 0x1B, 0x1B, 0x1B, 0x1B, 0x1B, 0x1B, 0x1B}; // 2 full



void setup(){

  lcd.begin(20, 4);  
  Serial.begin(9600);
  lcd.createChar(1, custom_1); 
//  lcd.createChar(2, custom_2);
  lcd.createChar(3, custom_3);
  lcd.createChar(4, custom_4);
  lcd.createChar(5, custom_5);
  lcd.createChar(6, custom_6);
  lcd.createChar(7, custom_7);
  lcd.createChar(8, custom_0);
}


void loop(){
    byte i;
  byte present = 0;
  byte type_s;
  byte data[12];
  byte addr[8];
  float celsius;
  int nbCelsius;

  if ( !ds.search(addr)) {
    Serial.println("No more addresses.");
    Serial.println();
    ds.reset_search();
    delay(250);
    return;
  }
  
  Serial.print("ROM =");
  for( i = 0; i < 8; i++) {
    Serial.write(' ');
    Serial.print(addr[i], HEX);
  }

  if (OneWire::crc8(addr, 7) != addr[7]) {
      Serial.println("CRC is not valid!");
      return;
  }
  Serial.println();
 
  // the first ROM byte indicates which chip
  switch (addr[0]) {
    case 0x10:
      Serial.println("  Chip = DS18S20");  // or old DS1820
      type_s = 1;
      break;
    case 0x28:
      Serial.println("  Chip = DS18B20");
      type_s = 0;
      break;
    case 0x22:
      Serial.println("  Chip = DS1822");
      type_s = 0;
      break;
    default:
      Serial.println("Device is not a DS18x20 family device.");
      return;
  } 

  ds.reset();
  ds.select(addr);
  ds.write(0x44,1);         // start conversion, with parasite power on at the end
  
  delay(1000);     // maybe 750ms is enough, maybe not
  // we might do a ds.depower() here, but the reset will take care of it.
  
  present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE);         // Read Scratchpad

//  Serial.print("  Data = ");
//  Serial.print(present,HEX);
//  Serial.print(" ");
  for ( i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = ds.read();
//    Serial.print(data[i], HEX);
    Serial.print(" ");
  }
//  Serial.print(" CRC=");
//  Serial.print(OneWire::crc8(data, 8), HEX);
  Serial.println();

  // convert the data to actual temperature

  int raw = (data[1] << 8) | data[0];
  if (type_s) {
    raw = raw << 3; // 9 bit resolution default
    if (data[7] == 0x10) {
      // count remain gives full 12 bit resolution
      raw = (raw & 0xFFF0) + 12 - data[6];
    }
  } else {
    byte cfg = (data[4] & 0x60);
    if (cfg == 0x00) raw = raw << 3;  // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw << 2; // 10 bit res, 187.5 ms
    else if (cfg == 0x40) raw = raw << 1; // 11 bit res, 375 ms
    // default is 12 bit resolution, 750 ms conversion time
  }
  celsius = (float)raw / 16.0;

//  fahrenheit = celsius * 1.8 + 32.0;
//  Serial.print("  Temperature = ");
//  Serial.print(celsius);
//  Serial.print(" C° ");
//  Serial.print(deg);
//  Serial.print(fahrenheit);
//  Serial.println(" Fahrenheit");
  
  
  lcd.setCursor(0,0);
  lcd.print("Station meteo");
  lcd.setCursor(0,1);
  lcd.print("by Sebastien");
  lcd.setCursor(0,2);
  lcd.print("Temp : ");
  lcd.print(celsius);
  lcd.write(6);
  lcd.print("C");

/* ===============
  Progress bar
  ================ */

  nbCelsius = (int)round(celsius * 10); // partie entiere de la temperature arondie au dixième
  lcd.setCursor(0,3); // on se place à la ligne de la "progress bar"

/* boucle pour l'affichage de la progress bar*/

  for (int i = 0; i < 20; i++){
    if (nbCelsius > 20){   // 20 dixième = 2°C
      if (i == 4 || i == 9 || i == 14 || i == 19){   // si 10, 20, 30 afficher un trait long
        lcd.write(5);
      } 
      else {
        lcd.write(4);
      }
      nbCelsius = nbCelsius - 20;
    }
    else{
            // lcd.print(nbCelsius);
      // creation du symbole à afficher
      uint8_t custom_2[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
      
      if ((nbCelsius = 20) && (i == 4 || i == 9 || i == 14 || i == 19)) {      //  cas unique ou themp == 10,20,30,40°C
         uint8_t custom_5[8] = { 0x03, 0x1B, 0x1B, 0x1B, 0x1B, 0x1B, 0x1B, 0x1B};
      }
      else {

        if (nbCelsius > 10){
          for (int j = 1; j < 8; j++){
            custom_2[j] = custom_2[j] + 24;
          }
          
          if (nbCelsius > 15){
            for (int k = 1; k < 8; k++){
              custom_2[k] = custom_2[k]+2;
            }
          }
        }
        
      for (int j = 1; j < 8; j++){
           custom_2[j] = 0x1A;
          }
                lcd.print("hello");
      lcd.createChar(2, custom_2);
      lcd.write(2);
      nbCelsius = 0;
      
//      switch (nbCelsius) {
//        case 2:
//          if (i == 4 || i == 9 || i == 14 || i == 19){   // si 10, 20, 30 afficher un trait long
//            lcd.write(5);
//          } else {
//            lcd.write(4);
//          }
//          nbCelsius = 0;
//          break;
//        case 1:
//          lcd.write(2);
//          nbCelsius = 0;
//          break;
//        default:
          lcd.write(8);
        //}
    }
    }
  }
 
}
