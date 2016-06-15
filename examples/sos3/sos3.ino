#include <Automaton.h>

Atm_led led[3];
Atm_timer timer[3];

const int pin = 4;
const int dotTime = 100;
const int dashTime = 300;
const int waitTime = 200;
const int longwaitTime = 300;
const int longerwaitTime = 1000;

void setup() {

  led[0].begin( pin )
    .blink(  dotTime, waitTime, 3 ) 
    .onFinish( timer[0], Atm_timer::EVT_START );
  timer[0].begin( longwaitTime ) 
    .onTimer( led[1], Atm_led::EVT_BLINK );

  led[1].begin( pin )
    .blink( dashTime, waitTime, 3 ) 
    .onFinish( timer[1], Atm_timer::EVT_START );
  timer[1].begin( longwaitTime )
    .onTimer( led[2], Atm_led::EVT_BLINK );

  led[2].begin( pin )
    .blink(  dotTime, waitTime, 3 ) 
    .onFinish( timer[2], Atm_timer::EVT_START );
  timer[2].begin( longerwaitTime )
    .onTimer( led[0], Atm_led::EVT_BLINK );    

  led[0].start();
}

void loop() {
  automaton.run();
}
