#pragma once

#include <Automaton.h>

#undef TINYMACHINE
#undef FACTORY
#undef STATE_TYPE
#undef MACHINE

#ifdef TINYMACHINE
#define MACHINE TinyMachine
#define FACTORY 0
#define STATE_TYPE tiny_state_t
#else
#define MACHINE Machine
#define FACTORY factory
#define STATE_TYPE state_t
#endif

// Detects a pulse (LOW -> HIGH ) on a digital pin with a minimum duration in ms
// On detection another machine is messaged or a callback is fired

class Atm_digital_in : public MACHINE {
 public:
  Atm_digital_in( void ) : MACHINE() {
#ifndef TINYMACHINE
    class_label = "PULSE";
#endif
  };

  short pin;
  int state_high, state_low;
  atm_timer_millis timer;
  bool _activeLow;
  atm_connector _connection[2];

  enum { IDLE, WAITH, VHIGH, WAITL, VLOW };
  enum { EVT_TIMER, EVT_HIGH, EVT_LOW, ELSE };
  enum { ACT_HIGH, ACT_LOW };
  enum { MODE_NULL, MODE_CALLBACK, MODE_MACHINE, MODE_TMACHINE, MODE_FACTORY };

  Atm_digital_in& begin( int attached_pin, int debounce = 20, bool activeLow = false, bool pullUp = false );
  int event( int id );
  void action( int id );
  int state( void );
  Atm_digital_in& onFlip( bool st, atm_cb_t callback, int idx = 0 );
  Atm_digital_in& onFlip( bool st, Machine& machine, int event = 0 );
  Atm_digital_in& onFlip( bool st, TinyMachine& machine, int event = 0 );
  Atm_digital_in& onFlip( bool st, const char* label, int event = 0 );
  Atm_digital_in& trace( Stream& stream );
};
