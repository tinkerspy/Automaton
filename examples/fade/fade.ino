#include <Atm_fade.h>
#include <Automaton.h>

int ledPin = 3;

Atm_fade led;

void setup() {
  led.begin( ledPin ).blink( 200 ).fade( 5 );
  led.state( led.START );
}

void loop() {
  led.cycle();
}
