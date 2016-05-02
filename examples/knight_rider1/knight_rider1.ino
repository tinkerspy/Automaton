#include <Automaton.h>
#include <Atm_led.h>

Atm_led led1, led2, led3, led4, led5, led6;
Factory factory;

void setup() {
  factory.add( led1.begin( 4 ).blink( 100, 1, 1 ).onFinish( led2 ) ); 
  factory.add( led2.begin( 5 ).blink( 100, 1, 1 ).onFinish( led3, led1 ) );
  factory.add( led3.begin( 6 ).blink( 100, 1, 1 ).onFinish( led4, led2 ) );
  factory.add( led4.begin( 7 ).blink( 100, 1, 1 ).onFinish( led5, led3 ) );
  factory.add( led5.begin( 8 ).blink( 100, 1, 1 ).onFinish( led6, led4 ) );
  factory.add( led6.begin( 9 ).blink( 100, 1, 1 ).onFinish( led5 ) );
  led1.trigger( led1.EVT_BLINK );
}

void loop() {
  factory.cycle();
}

