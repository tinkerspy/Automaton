#include <Automaton.h>
#include "blink.h"

Blink led;

void setup()
{
  led.begin( 3, 200 );
}

void loop()
{
  led.cycle();
}

