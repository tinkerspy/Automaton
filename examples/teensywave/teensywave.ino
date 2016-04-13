#include <Automaton.h>
#include <Atm_teensywave.h>
#include <Atm_button.h>

// A state machine generates simple waveforms on Teensy 3.1/LC (which has true analog out)
// Attach a button to pin 11 to cycle thru SINE (SN) > SAW (SW) > SAW REVERSE (SR) > TRIANGLE (TR) > SQUARE (SQ)

int pinOut = A14; // Teensy 3.1 analog output pin (LC uses A12)
int pinIn = 11;   // Button attached to pin 11 (pullup)

Atm_teensywave wave;
Atm_button btn;
Factory factory;

void btnHandler( int press ) {
  if ( press ) {
    wave.trigger( wave.EVT_TOGGLE );
  }
}

void setup()
{
  // 314 steps, 50 usec period, start machine in SN (Sine Wave) state
  wave.begin( pinOut, 314, 50 ); 
  btn.begin( pinIn, btnHandler ).autoPress( 10000, 1 );
  factory.add( wave ).add( btn );
  wave.trigger( wave.EVT_TOGGLE );
}

void loop()
{
  factory.cycle();
}
