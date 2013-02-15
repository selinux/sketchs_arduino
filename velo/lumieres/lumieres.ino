#ifndef Arduino_h
	#include <Arduino.h>
#endif

/* Seuil de détection */
const uint16_t THRESHOLD = 90;


const int ledPinD =  10;      // the number of the LED pin
const int ledPinG =  11;      // the number of the LED pin
const int D = 2;
const int G = 3;

long previousMillis = 0;        // will store last time LED was updated
long interval = 500;           // interval at which to blink (milliseconds)
int ledState = LOW;             // ledState used to set the LED
long debugD;
long debugG;

/* setup() */
void setup() {

  /* Initialise le port série à 115200 bauds */
  Serial.begin(115200);

  pinMode(ledPinD, OUTPUT);
  pinMode(ledPinG, OUTPUT);
}

/* loop() */
void loop() {

  unsigned long currentMillis = millis();

   
if(currentMillis - previousMillis > interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;  

    // if the LED is off turn it on and vice-versa:
       if (ledState == LOW)
          ledState = HIGH;
       else
          ledState = LOW;
          
    // valide l'état de la LED Droite si besoin
        if (touch_measure(D) > THRESHOLD) {
           digitalWrite(ledPinD, ledState);
        }
        else { digitalWrite(ledPinD, LOW); }

    // valide l'état de la LED Gauche si besoin
        if (touch_measure(G) > THRESHOLD) {
           digitalWrite(ledPinG, ledState);
        }
        else { digitalWrite(ledPinG, LOW); }
  }    

  debugD = touch_measure(D);
  debugG = touch_measure(G);
  Serial.print(debugD);
  Serial.print("    ");
  Serial.println(debugG);
}

/**
 * Mesure la capacité électrique présente sur une broches digitale
 *
 * Principe : Place la broche au 0v, puis compte le nombre de cycles requis avant que la broche ne commute.
 * Ce systéme tire parti du temps de charge d'un condensateur au travers d'une résistance de forte valeur (>1M ohms).
 *
 * @param measurePin Broche sur laquelle effectuer la mesure
 * @return Valeur comprise entre 0 (= pas de capacitance) et 255 (= "grosse" capacitance)
 *
 * Remarque : une résistance >1M ohms entre la broche et +VCC est obligatoire !
 */
uint8_t touch_measure(uint8_t measurePin){

  /* Registre bas-niveau, DDR = port de configuration, PORT = port de sortie, PIN = port d'entrée */
  uint8_t xport = digitalPinToPort(measurePin);
  volatile uint8_t *ddr = portModeRegister(xport);
  volatile uint8_t *port = portOutputRegister(xport);
  volatile uint8_t *pin = portInputRegister(xport);

  /* Résultat de la mesure, Bitmask de la broche à mesurer */
  uint8_t cycles, bitmask = digitalPinToBitMask(measurePin);

  /* Décharge la capacité en contact avec la broche */
  *port &= ~(bitmask);     // Place la broche à LOW
  *ddr |= bitmask;         // Place la broche en sortie
  delayMicroseconds(1000); // Attend pour être sur que la capacité est déchargé

  /* Place la broche en entrée, sans résistance de pull-up ! */
  /* (La résistance de >1M ohms externe servira de résistance de pull-up) */
  *ddr &= ~(bitmask);

  /* Mesure le nombre de cylces CPU requis avant que la broche ne commute */
  for(cycles = 0; cycles < 256; ++cycles){
    if (*pin & bitmask) break; // Si la broche a commuter on quitte la boucle
  }

  /* Re-décharge la capacité en contact avec la broche
   * afin d'éviter tout parasitages d'une autre mesure sur une autre broche.
   * Dans le cas contraire il serait impossible de manipuler plus d'une touche "tactile" */
  *port &= ~(bitmask);
  *ddr |= bitmask;

  /* Retourne le résultat */
  return cycles;
}

