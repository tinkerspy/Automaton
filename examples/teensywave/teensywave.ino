#include <Automaton.h>
#include <Atm_teensywave.h>
#include <Atm_button.h>

// A state machine generates simple waveforms on Teensy 3.1
// Attach a button to pin 11 to cycle thru SINE (SN) > SAW (SW) > SQUARE (SQ)

int pinOut = A14;
int pinIn = 11;

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
  wave.begin( pinOut, 314, 50 ).state( wave.START_SN ); // Start in SINE mode (SN)
  btn.begin( pinIn, btnHandler );
  factory.add( wave ).add( btn );
}

void loop()
{
  factory.cycle();
}

