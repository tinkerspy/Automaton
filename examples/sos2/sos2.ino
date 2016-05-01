#include <Automaton.h>
#include <Atm_led.h>
#include <Atm_timer.h>
#include <Atm_step.h>

Atm_led dot, dash;
Atm_timer stepTimer;
Atm_step step;
Factory factory;

const int pin = 4;
const int dotTime = 100;
const int dashTime = 300;
const int waitTime = 200;
const int longwaitTime = 300;
const int longerwaitTime = 1000;

void setup() {
  
  // Define two leds (patterns) and add them to the factory
  factory.add(  dot.begin( 3 ).blink(  dotTime, waitTime, 3 ) );
  factory.add( dash.begin( 3 ).blink( dashTime, waitTime, 3 ) );
  
  // Define a timer and add it to the factory
  factory.add( stepTimer.begin( 1700 ).repeat( ATM_COUNTER_OFF ) );
  
  // Define a step sequencer, add it to the factory and link it to the leds we defined earlier
  factory.add( step.begin()
    .onStep( 0,  dot, Atm_led::EVT_BLINK )
    .onStep( 1, dash, Atm_led::EVT_BLINK )
    .onStep( 2,  dot, Atm_led::EVT_BLINK )
  );
  
  // Set up the timer to drive the step sequencer
  stepTimer.onTimer( step, Atm_step::EVT_STEP );  
  
  // Start the timer
  stepTimer.trigger( Atm_timer::EVT_ON );
}

void loop() {
  factory.cycle();
}
