#include <Automaton.h>

// Turning a pot on A0 will change a led gauge on pins 4, 5, 6, 7, 8, 9

Atm_comparator cmp;
Atm_led led[6];
Atm_step step;

static uint16_t threshold_list[] = { 100, 300, 500, 700, 900, 1000 }; 
static short pin_list[] = { 4, 5, 6, 7, 8, 9 };

void setup() {
    
  cmp.begin( A0, 50 )
    .threshold( threshold_list, sizeof( threshold_list ), true )
    .onChange( true, step, Atm_step::EVT_STEP )
    .onChange( false, step, Atm_step::EVT_BACK );
      
  step.begin();
  for ( short i = 0; i <= 5; i++ ) {
    led[i].begin( pin_list[i] );
    step.onStep( i, led[i], Atm_led::EVT_TOGGLE );    
  }
}

void loop() {
  automaton.run();
}
