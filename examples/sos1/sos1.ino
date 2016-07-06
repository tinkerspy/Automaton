#include <Automaton.h>

Atm_led led;

const int pin = 4;
const int dotTime = 100;
const int dashTime = 300;
const int waitTime = 200;
const int longwaitTime = 500;
const int longerwaitTime = 1000;

void setup() {
  led.begin( pin );
}

void loop() {
  // Set the led to blink 3 times and trigger it
  led.blink( dotTime, waitTime, 3 ).start();
  
  // Loop until the blinking has finished
  while ( led.cycle().state() ); 
    
  // Cycle idly for a while
  led.cycle( longwaitTime );
       
  led.blink( dashTime, waitTime, 3 ).start();
  while ( led.cycle().state() );
  led.cycle( longwaitTime );  
   
  led.blink( dotTime, waitTime, 3 ).start();
  while ( led.cycle().state() );
  led.cycle( longerwaitTime );   
}
