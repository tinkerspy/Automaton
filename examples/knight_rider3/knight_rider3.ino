#include <Automaton.h>
#include <Atm_button.h>
#include "Atm_sweep.h"

Atm_sweep sweep;
Atm_button button;
Factory factory;

void setup() {
  factory
    .add( button.begin( 8 ) )
    .add( sweep.begin( 3, 4, 5, 10, 11, 12 ).speed( 50 ) );
  button.onPress( sweep, sweep.EVT_TOGGLE );
}

void loop() {
  factory.cycle();
}

