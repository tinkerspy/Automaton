#include <Automaton.h>

// This example demonstrates running state machines logging their state changes to the serial port

int ledPin = 4;
int buttonPin = 2;

Atm_led led;
Atm_button btn;
Appliance app;

void setup() {
  Serial.begin( 9600 );
  
  app.component(
    led.begin( ledPin )
      .blink( 1000 )
      .trace( Serial )
  );
  
  app.component(
    btn.begin( buttonPin )
      .onPress( led, led.EVT_TOGGLE )
      .trace( Serial )
  );
}

void loop() {
  app.run();
}
