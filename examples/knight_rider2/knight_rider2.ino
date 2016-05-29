#include <Automaton.h>
#include "Atm_sweep.h"

Atm_sweep sweep;
Atm_analog pot;
Appliance app;

void setup() {

  app.component( 
    sweep.begin( 4, 5, 6, 7, 8, 9 )
      .speed( 50 ) 
      .trigger( sweep.EVT_START )
  );

  app.component( 
    pot.begin( A0 )
      .range( 10, 200 )
      .onChange( []( int idx, int v, int up ) {
        sweep.speed( v );
      })
  );
  
}

void loop() {
  app.run();
}
