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

void sw( const char label[], const char current[], const char next[], const char trigger[], uint32_t runtime, uint32_t cycles ) {
  Serial.print( millis() );
  Serial.print( " Switching " );
  Serial.print( label );
  Serial.print( " from state " );
  Serial.print( current );
  Serial.print( " to " );
  Serial.print( next );
  Serial.print( " on trigger " );
  Serial.print( trigger );
  Serial.print( " (" );
  Serial.print( cycles );
  Serial.print( " cycles in " );
  Serial.print( runtime );
  Serial.println( " ms)" );
}

void cb( int press ) {

}

void setup() {
  Serial.begin( 9600 );
  delay( 1000 );
  Serial.println( "start" );
  led1.begin( led1Pin ).blink( 200 ).state( led1.START );
  led2.begin( led2Pin ).blink( 300 ).state( led2.START );
  led1.label( "LED1" ).onSwitch( sw, "IDLE\0ON\0START\0BLINK_OFF", "EVT_ON_TIMER\0EVT_OFF_TIMER\0EVT_COUNTER\0ELSE" );
  led2.label( "LED2" ).onSwitch( sw, "IDLE\0ON\0START\0BLINK_OFF", "EVT_ON_TIMER\0EVT_OFF_TIMER\0EVT_COUNTER\0ELSE" );
  btn.begin( buttonPin, cb ).onSwitch( sw,
    "IDLE\0WAIT\0PRESSED\0REPEAT\0RELEASE\0LIDLE\0LWAIT\0LPRESSED\0LRELEASE\0WRELEASE\0AUTO",
    "EVT_LMODE\0EVT_TIMER\0EVT_DELAY\0EVT_REPEAT\0EVT_PRESS\0EVT_RELEASE\0EVT_COUNTER\0EVT_AUTO\0ELSE" ); 
  factory.add( led1).add( led2 ).add( btn );  
}

void loop() {
  factory.cycle();
}
