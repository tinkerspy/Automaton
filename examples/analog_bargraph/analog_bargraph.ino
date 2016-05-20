#include <Automaton.h>

// six leds on 4, 5, 6, 7, 8, 9 and a pot on A0

Atm_comparator cmp;
Atm_led led[6];
Atm_step step;
Factory factory;

static uint16_t threshold_list[] = { 100, 300, 500, 700, 900, 1000 }; 
static short pin_list[] = { 4, 5, 6, 7, 8, 9 };

void setup() 
{
  factory.add( cmp.begin( A0, 50 ) );
  cmp.threshold( threshold_list, sizeof( threshold_list ) / sizeof( threshold_list[0] ), true )
    .onUp( step, Atm_step::EVT_STEP )
    .onDown( step, Atm_step::EVT_BACK );
    
  factory.add( step.begin() );
  for ( short i = 0; i <= 5; i++ ) {
    factory.add( led[i].begin( pin_list[i] ) );
    step.onStep( i, led[i], Atm_led::EVT_TOGGLE );    
  }
}

void loop()
{
  factory.cycle();
}
