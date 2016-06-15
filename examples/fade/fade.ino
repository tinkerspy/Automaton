#include <Automaton.h>

int led1Pin = 5;
int led2Pin = 6;

Atm_fade led1, led2;

void setup() {
  led1.begin( led1Pin ).blink( 200 ).fade( 5 );
  led2.begin( led2Pin ).blink( 500 ).fade( 10 );
  led1.start();
  led2.start();
}

void loop() {
  led1.cycle();
  led2.cycle();
}
