#pragma once

#include <Automaton.h>

#undef TINYMACHINE
#undef STATE_TYPE
#undef MACHINE

#ifdef TINYMACHINE
#define MACHINE TinyMachine
#define STATE_TYPE tiny_state_t
#else
#define MACHINE Machine
#define STATE_TYPE state_t
#endif

// Digital pin with a minimum duration in ms
// On detection another machine is messaged or a callback is fired

class Atm_digital : public MACHINE {
 public:
  Atm_digital( void ) : MACHINE() {};

  short pin;
  int state_high, state_low;
  atm_timer_millis timer;
  bool _activeLow;
  atm_connector _connection[2];
  int8_t _indicator;
  bool _indicatorActiveLow;

  enum { IDLE, WAITH, VHIGH, WAITL, VLOW };
  enum { EVT_TIMER, EVT_HIGH, EVT_LOW, ELSE };
  enum { ACT_HIGH, ACT_LOW };

  Atm_digital& begin( int attached_pin, int debounce = 20, bool activeLow = false, bool pullUp = false );
  int event( int id );
  void action( int id );
  int state( void );
  Atm_digital& onFlip( bool status, atm_cb_t callback, int idx = 0 );
  Atm_digital& onFlip( bool status, Machine& machine, int event = 0 );
  Atm_digital& indicator( int led, bool activeLow = false );
  Atm_digital& trace( Stream& stream );
};
