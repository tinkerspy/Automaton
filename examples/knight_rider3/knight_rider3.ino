#include <Automaton.h>
#include <Atm_button.h>
#include "Atm_sweep.h"

Atm_sweep sweep;
Atm_button button;
Factory factory;

void setup() {
  factory
    .add( button.begin( 2 ) )
    .add( sweep.begin( 4, 5, 6, 7, 8, 9 ).speed( 50 ) );
  button.onPress( sweep, sweep.EVT_TOGGLE );
}

void loop() {
  factory.cycle();
}

