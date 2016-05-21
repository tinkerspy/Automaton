#include <Automaton.h>

// Start by creating a bunch of state machines

Atm_led led1, led2, led3; // Three Automaton led machines
Atm_button btn; // An Automaton button machine
Appliance app; // And finally a appliance to put them in

void setup() {
  // Initialize the led machines at different rates
  app.component( led1.begin( 4 ).blink( 20 ) );
  app.component( led2.begin( 5 ).blink( 10 ).pause( 1000 ) );
  app.component( led3.begin( 6 ).blink( 10 ).pause( 200 ) );

  // Set up a button to send a trigger to all machines in the .LED class
  app.component( btn.begin( 2 ) FIXME
    .onPress( ".LED", Atm_led::EVT_TOGGLE_BLINK )
  );
  
  // Start the blinking
  app.trigger( ".LED", Atm_led::EVT_BLINK );
}

// Run the app from the Arduino loop()
// Press the button to toggle the leds on and off

void loop() {
  app.run();
}
