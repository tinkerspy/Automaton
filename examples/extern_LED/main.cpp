#include <automaton.h>
#include <atm_led_mcp.h>
#include "Adafruit_MCP23017.h"


Adafruit_MCP23017 ioext;
Atm_led_mcp led(ioext);


void setup() {
  ioext.begin();
  led.begin(6, true);
  led.On();
}

void loop() {
  automaton.run();
}
