#include <Automaton.h>
#include "Atm_blink.h"

Atm_blink led;

void setup() {
  led.begin( LED_BUILTIN, 200 );        // Setup a blink machine
  led.trigger( led.EVT_ON );  // Turn it on
}

void loop() {
  automaton.run();
}

