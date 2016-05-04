#include <Automaton.h>

// Start by creating a bunch of state machines

Atm_led led1, led2, led3; // Three Automaton led machines
Atm_button btn; // An Automaton button machine
Factory factory; // And finally a factory to house them in

void setup() 
{
  // Initialize the led machines at different rates
  led1.begin( 4 ).blink( 20 );
  led2.begin( 5 ).blink( 10 ).pause( 1000 );
  led3.begin( 6 ).blink( 10 ).pause( 200 );

  // Set up a button to send a trigger to all machines in the .LED class
  btn.begin( 2 ).onPress( ".LED", Atm_led::EVT_TOGGLE_BLINK );
  
  // Add the initialized machines to the factory
  factory.add( led1 ).add( led2 ).add( led3 ).add( btn );

  // Start the blinking
  factory.trigger( ".LED", Atm_led::EVT_BLINK );
}

// Run the factory from the Arduino loop()
// Press the button to toggle the leds on and off

void loop() 
{
  factory.cycle();
}
