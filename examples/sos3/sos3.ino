#include <Automaton.h>

Atm_led led[3];
Atm_timer timer[3];
Factory factory;

const int pin = 4;
const int dotTime = 100;
const int dashTime = 300;
const int waitTime = 200;
const int longwaitTime = 300;
const int longerwaitTime = 1000;

void setup() {

  factory.add( led[0].begin( pin ).blink(  dotTime, waitTime, 3 ) );
  factory.add( timer[0].begin( longwaitTime ) );
  led[0].onFinish( timer[0], Atm_timer::EVT_ON );
  timer[0].onTimer( led[1], Atm_led::EVT_BLINK );  

  factory.add( led[1].begin( pin ).blink( dashTime, waitTime, 3 ) );
  factory.add( timer[1].begin( longwaitTime ) );
  led[1].onFinish( timer[1], Atm_timer::EVT_ON );
  timer[1].onTimer( led[2], Atm_led::EVT_BLINK );  
  
  factory.add( led[2].begin( pin ).blink(  dotTime, waitTime, 3 ) );
  factory.add( timer[2].begin( longerwaitTime ) );
  led[2].onFinish( timer[2], Atm_timer::EVT_ON );
  timer[2].onTimer( led[0], Atm_led::EVT_BLINK );

  led[0].trigger( Atm_led::EVT_BLINK );
}

void loop() {
  factory.cycle();
}
