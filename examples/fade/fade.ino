#include <Automaton.h>
#include <Atm_fade.h>

int ledPin = 5;

Atm_fade led;

void setup() {
  led.begin( ledPin ).blink( 200 ).fade( 5 );
  led.trigger( led.EVT_BLINK );
}

void loop() {
  led.cycle();
}