#include <Automaton.h>
#include <Atm_button.h>
#include <Atm_led.h>
#include <Atm_timer.h>
#include <Atm_step.h>

Atm_led led[6];
Atm_timer timer;
Atm_step step;
Factory factory;

// Timer drives step sequencer in sweep mode, step sequencer blinks leds.

short evt = Atm_led::EVT_BLINK;
static short pin_list[] = { 9, 8, 7, 6, 5, 4 };
int blink_time = 70;
int interval_time = 50;

void setup() {
  factory
    .add( step.begin().trigger( Atm_step::EVT_SWEEP ) )
    .add( timer.begin( interval_time ).onTimer( step, Atm_step::EVT_STEP ).repeat( ATM_COUNTER_OFF ) );
  for ( short i = 0; i <= 5; i++ ) {
    factory.add( led[i].begin( pin_list[i] ).blink( blink_time, 1, 1 ) ); // pause = 0 still doesn't work!
    step.onStep( i, led[i], evt );
  }
  // Move last led from step 5 to step 7 to make sweep work properly!
  step.onStep( 5 ).onStep( 7, led[5], evt );
  timer.trigger( timer.EVT_ON );
}

void loop() {
  factory.cycle();
}
