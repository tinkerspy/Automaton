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

void cb( int press, int idx, uint16_t cnt ) {

}

void setup() {
  Serial.begin( 9600 );
  led1.begin( led1Pin ).blink( 200 ).trigger( led1.EVT_BLINK );
  led2.begin( led2Pin ).blink( 300 ).trigger( led1.EVT_BLINK );
  led1.trace( Serial ).label( "LED1" );
  led2.trace( Serial ).label( "LED2" );
  btn.begin( buttonPin ).onPress( cb ).trace( Serial );
  factory.add( led1).add( led2 ).add( btn );  
}

void loop() {
  factory.cycle();
}