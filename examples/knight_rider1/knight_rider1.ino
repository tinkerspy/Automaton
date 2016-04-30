#include <Automaton.h>
#include <Atm_led.h>

Atm_led led1, led2, led3, led4, led5, led6;
Factory factory;

void setup() {
  Serial.begin( 9600 );
  led1.trace( Serial );
  factory.add( led1.begin( 3  ).blink( 100, 1, 1 ).chain( led2 ) ); // pause = 0 does not work!!!!!!!!!!
  factory.add( led2.begin( 4  ).blink( 100, 1, 1 ).chain( led3, led1 ) );
  factory.add( led3.begin( 5  ).blink( 100, 1, 1 ).chain( led4, led2 ) );
  factory.add( led4.begin( 10 ).blink( 100, 1, 1 ).chain( led5, led3 ) );
  factory.add( led5.begin( 11 ).blink( 100, 1, 1 ).chain( led6, led4 ) );
  factory.add( led6.begin( 12 ).blink( 100, 1, 1 ).chain( led5 ) );
  led1.trigger( led1.EVT_BLINK );
}

void loop() {
  factory.cycle();
}

