#include <Automaton.h>
#include <Atm_led.h>
#include <Atm_button.h>

Atm_led led1, led2;
Atm_button btn;
Factory factory;

void sw( const char label[], int current, int next, int trigger, uint32_t runtime, uint32_t cycles ) {
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
  Serial.print( "/" );
  Serial.print( runtime );
  Serial.println( " cycles/ms)" );
}

void cb( int press ) {

}

void setup() {
  Serial.begin( 9600 );
  delay( 1000 );
  Serial.println( "start" );
  led1.begin( 3 ).duration( 200 ).state( led1.START ).on_switch( sw );
  led2.begin( 4 ).duration( 300 ).state( led2.START ).label( "LED2" ).on_switch( sw );
  btn.begin( 14, cb ).on_switch( sw ); 
  factory.add( led1).add( led2 ).add( btn );  
}

void loop() {
  factory.cycle();
}

