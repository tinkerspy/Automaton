
#ifndef Atm_pulse_h
#define Atm_pulse_h

#include <Automaton.h>

#define MACHINE Machine
#undef TINYMACHINE

// Detects a pulse (LOW -> HIGH ) on a digital pin with a minimum duration in ms
// On detection another machine is messaged or a callback is fired

class Atm_pulse : public MACHINE {
 public:
  Atm_pulse( void ) : MACHINE() {
#ifndef TINYMACHINE
    class_label = "PULSE"; 
#endif
  };

  short pin;
  int state_high, state_low;
  atm_timer_millis timer;
  bool _activeLow;
  atm_connector _onpulse, _onrelease;

  enum { IDLE, WAIT, PULSE };
  enum { EVT_TIMER, EVT_HIGH, EVT_LOW, ELSE };
  enum { ACT_PULSE, ACT_RELEASE };
  enum { MODE_NULL, MODE_CALLBACK, MODE_MACHINE, MODE_TMACHINE, MODE_FACTORY };

  Atm_pulse& begin( int attached_pin, int minimum_duration = 20, bool activeLow = false, bool pullUp = false );
  int event( int id );
  void action( int id );
  Atm_pulse& onPulse( atm_cb_t callback, int idx = 0 );
  Atm_pulse& onPulse( Machine& machine, int event = 0 );
  Atm_pulse& onPulse( TinyMachine& machine, int event = 0 );
  Atm_pulse& onPulse( const char* label, int event = 0 );
  Atm_pulse& onRelease( atm_cb_t callback, int idx = 0 );
  Atm_pulse& onRelease( Machine& machine, int event = 0 );
  Atm_pulse& onRelease( TinyMachine& machine, int event = 0 );
  Atm_pulse& onRelease( const char* label, int event = 0 );
  Atm_pulse& trace( Stream& stream );
};

#endif
