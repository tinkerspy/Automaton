#include <Automaton.h>
#include "Atm_blink.h"

Atm_blink led;

void setup()
{
  led.begin( 3, 200 );
}

void loop()
{
  led.cycle();
}

