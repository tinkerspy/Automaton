#include <Automaton.h>

// Start by creating a bunch of state machines

Atm_led led1, led2, led3; // Three Automaton led machines
Atm_button btn; // An Automaton button machine
Atm_fan fan; // To split the trigger in 3
Appliance app; // And finally a appliance to put them in

void setup() {
  // Initialize the led machines at different rates
  app.component( led1.begin( 4 ).blink( 100, 100 ) );
  app.component( led2.begin( 5 ).blink( 200, 200 ) );
  app.component( led3.begin( 6 ).blink( 400, 400 ) );

  // Send one event to many
  app.component(
    fan.begin()
      .onInput( led1, led1.EVT_TOGGLE_BLINK )
      .onInput( led2, led2.EVT_TOGGLE_BLINK )
      .onInput( led3, led3.EVT_TOGGLE_BLINK )
  );

  // Button triggers the fan
  app.component( 
    btn.begin( 2 ) 
      .onPress( fan, fan.EVT_INPUT )
  );
  
  // Start the blinking
  fan.trigger( fan.EVT_INPUT );
}

// Run the app from the Arduino loop()
// Press the button to toggle the leds on and off

void loop() {
  app.run();
}
