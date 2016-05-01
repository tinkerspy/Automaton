#include <Automaton.h>
#include <Atm_led.h>

Atm_led led;

const int pin = 4;
const int dotTime = 100;
const int dashTime = 300;
const int waitTime = 200;
const int longwaitTime = 500;
const int longerwaitTime = 1000;

void setup() {
  led.begin( 3 );
}

void loop() {
  // Set the led to blink 3 times and trigger it
  led.blink( dotTime, waitTime, 3 ).trigger( Atm_led::EVT_BLINK );
  
  // Loop until the blinking has finished
  while ( led.state() ) 
    led.cycle();
    
  // Cycle idly for a while
  led.cycle( longwaitTime );
       
  led.blink( dashTime, waitTime, 3 ).trigger( Atm_led::EVT_BLINK );
  while ( led.state() ) 
    led.cycle();
  led.cycle( longwaitTime );  
   
  led.blink( dotTime, waitTime, 3 ).trigger( Atm_led::EVT_BLINK );
  while ( led.state() ) 
    led.cycle();
  led.cycle( longerwaitTime );   
}
