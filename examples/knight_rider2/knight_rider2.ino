#include <Automaton.h>
#include "Atm_sweep.h"

Atm_sweep sweep;
Atm_button button;

void setup() {

  sweep.begin( 4, 5, 6, 7, 8, 9 )
    .speed( 50 ) 
    .trigger( sweep.EVT_START );

  button.begin( 2 )
    .onPress( sweep, sweep.EVT_TOGGLE );
      
}

void loop() {
  automaton.run();
}
