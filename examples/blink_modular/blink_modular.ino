#include <Automaton.h>
#include "Atm_blink.hpp"

Atm_blink led;

void setup()
{
  led.begin( 4, 200 );        // Setup a blink machine
  led.trigger( led.EVT_ON );  // Turn it on
}

void loop()
{
  led.cycle();
}

