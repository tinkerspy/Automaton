#include <Automaton.h>

// This example demonstrates running state machines logging their state changes to the serial port

int ledPin = LED_BUILTIN;
int buttonPin = 2;

Atm_led led;
Atm_button button;

void setup() {
  Serial.begin( 9600 );
  led.trace( Serial );
  button.trace( Serial );
  
  led.begin( ledPin )
    .blink( 1000 ); 
  
  button.begin( buttonPin )
    .onPress( led, led.EVT_TOGGLE );
}

void loop() {
  automaton.run();
}
