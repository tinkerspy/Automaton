#include <Automaton.h>

// This example demonstrates running state machines logging their state changes to the serial port

int ledPin = 4;
int buttonPin = 2;

Atm_led led;
Atm_button button;
Appliance app;

void setup() {
  Serial.begin( 9600 );
  led.trace( Serial );
  button.trace( Serial );
  
  app.component(
    led.begin( ledPin )
      .blink( 1000 )
  );
  
  app.component(
    button.begin( buttonPin )
      .onPress( led, led.EVT_TOGGLE )
  );
}

void loop() {
  app.run();
}
