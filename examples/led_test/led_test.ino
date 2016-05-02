#include <Automaton.h>
#include <Atm_led.h>
#include <Atm_button.h>

// This example demonstrates running state machines logging their state changes to the serial port

int ledPin = 4;
int buttonPin = 2;

Atm_led led;
Atm_button btn;
Factory factory;

void setup() {
  Serial.begin( 9600 );
  led.begin( ledPin ).blink( 1000 );
  btn.begin( buttonPin ).onPress( led, led.EVT_TOGGLE );
  led.trace( Serial ).label( "LED1" );
  btn.trace( Serial ).label( "BTN1" );
  factory.add( led ).add( btn );  
}

void loop() {
  factory.cycle();
}