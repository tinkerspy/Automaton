#include <Automaton.h>
#include <Atm_led.h>
#include <Atm_fade.h>
#include <Atm_button.h>

// Start by creating a bunch of state machines

//Atm_led led1, led2, led3; // Two Atm_led machines
Atm_fade led1, led2,led3; // An Atm_fade machine
Atm_button btn; // An Atm_button machine
Factory factory; // And finally a factory to house them in

// This function is called by the Atm_button machine whenever a button press or release is detected

void btn_change( int press ) 
{
  if ( press ) {
    led1.toggle( led1.IDLE, led1.START );
    led2.toggle( led2.IDLE, led2.START );
    led3.toggle( led3.IDLE, led3.START );
  }
}

// Configure the machines, add them to the factory and set the initial state of the LEDs to on

void setup() 
{
  //Serial.begin(9600);delay(1000);Serial.println( "start" );
  // Initialize the machines
  led1.begin( 3 ).duration( 200 ).state( led1.START );
  led2.begin( 4 ).duration( 100, 1000 ).fade( 5 ).state( led2.START );
  led3.begin( 5 ).duration( 100, 200 ).fade( 5 ).repeat( 10 ).state( led3.START );
  btn.begin( 14, btn_change );
  
  // Add the initialized machines to the factory
  factory.add( led1 );
  factory.add( led2 );
  factory.add( led3 );
  factory.add( btn );
}

// Run the factory from the Arduino loop()

void loop() 
{
  factory.cycle();
}

