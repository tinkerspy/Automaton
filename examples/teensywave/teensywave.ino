#include <Automaton.h>
#include <Atm_teensywave.h>
#include <Atm_button.h>

// A state machine generates simple waveforms on Teensy 3.1/LC (which has true analog out)
// Attach a button to pin 11 to cycle thru SINE (SN) > SAW (SW) > SQUARE (SQ)

int pinOut = A14; // Teensy 3.1 analog output pin (LC uses A12)
int pinIn = 11;   // Button attached to pin 11 (pullup)

Atm_teensywave wave;
Atm_button btn;
Factory factory;

void btnHandler( int press ) {
  if ( press ) {
    wave.signalWrite( wave.SIG_TOGGLE );
  }
}

void setup()
{
  // 314 steps, 50 usec period, start machine in SN (Sine Wave) state
  wave.begin( pinOut, 314, 50 ).state( wave.START_SN ); 
  btn.begin( pinIn, btnHandler );
  factory.add( wave ).add( btn );
}

void loop()
{
  factory.cycle();
}

