#include <Automaton.h>
#include <Atm_led.h>
#include <Atm_button.h>

// This example demonstrates running state machines logging their state changes to the serial port

int led1Pin = 3;
int led2Pin = 4;
int buttonPin = 11;

Atm_led led1, led2;
Atm_button btn;
Factory factory;

void cb( int press ) {

}

void setup() {
  Serial.begin( 9600 );
  led1.begin( led1Pin ).blink( 200 ).state( led1.START );
  led2.begin( led2Pin ).blink( 300 ).state( led2.START );
  led1.onSwitch( atm_serial_debug::onSwitch ).label( "LED1" );
  led2.onSwitch( atm_serial_debug::onSwitch ).label( "LED2" );
  btn.begin( buttonPin, cb ).onSwitch( atm_serial_debug::onSwitch );
  factory.add( led1).add( led2 ).add( btn );  
}

void loop() {
  factory.cycle();
}
