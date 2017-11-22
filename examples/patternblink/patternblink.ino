#include <Arduino.h>
#include <Automaton.h>
#include <Atm_led.hpp>

Atm_led led;
byte bpattern[] = { 1,1,1,0,2,2,2,0,1,1,1,0,0 };

Atm_led led2;
byte bpattern2[] = { 1,1,0 };


void setup() {
	Serial.begin(9600);

  	led2
  	.blink(100,100)
  	.begin(5, false, bpattern2, sizeof(bpattern2))
	.onFinish(led,led.EVT_BLINK) 
	;

	led
	.blink(100,100)
	.begin(4, false, bpattern, sizeof(bpattern))
	//.trace(Serial)
	.onFinish(led2,led.EVT_BLINK) 
   	.trigger( led.EVT_BLINK)
   	;
}

void loop() {
    automaton.run();
}
