#include <Automaton.h>

// Start by creating a bunch of state machines

Atm_led led1, led2, led3; // Three Automaton led machines
Atm_button btn; // An Automaton button machine
Atm_fan fan; // To split the trigger in 3

void setup() {
  // Initialize the led machines at different rates
  led1.begin( 4 ).blink( 100, 100 );
  led2.begin( 5 ).blink( 200, 200 );
  led3.begin( 6 ).blink( 400, 400 );

  // Send one event to many
  fan.begin()
    .onInput( led1, led1.EVT_TOGGLE_BLINK )
    .onInput( led2, led2.EVT_TOGGLE_BLINK )
    .onInput( led3, led3.EVT_TOGGLE_BLINK );
      
  // Button triggers the fan
  btn.begin( 2 ) 
    .onPress( fan, fan.EVT_INPUT );
  
  // Start the blinking
  fan.trigger( fan.EVT_INPUT );
}

// Run the app from the Arduino loop()
// Press the button to toggle the leds on and off

void loop() {
  automaton.run();
}
