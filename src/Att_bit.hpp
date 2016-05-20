#pragma once

#include <Automaton.h>

#define TINYMACHINE
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

class Att_bit : public MACHINE {
 public:
  Att_bit( void ) : MACHINE() {
#ifndef TINYMACHINE
    class_label = "BIT";
#endif
  };

  state_t _last_state;
  atm_connector _connector[4];
  int8_t _indicator;
  bool _indicatorActiveLow;

  enum { OFF, ON };                                       // STATES
  enum { EVT_ON, EVT_OFF, EVT_TOGGLE, EVT_INPUT, ELSE };  // EVENTS
  enum { ACT_ON, ACT_OFF, ACT_INPUT };                    // ACTIONS

  Att_bit& begin( bool initialState = false );
  Att_bit& onFlip( bool status, atm_cb_t callback, int idx = 0 );
  Att_bit& onFlip( bool status, Machine& machine, state_t event = 0 );
  Att_bit& onFlip( bool status, const char* label, state_t event = 0 );
  Att_bit& onFlip( bool status, TinyMachine& machine, state_t event = 0 );
  Att_bit& onInput( bool status, atm_cb_t callback, int idx = 0 );
  Att_bit& onInput( bool status, Machine& machine, state_t event = 0 );
  Att_bit& onInput( bool status, const char* label, state_t event = 0 );
  Att_bit& onInput( bool status, TinyMachine& machine, state_t event = 0 );
  Att_bit& indicator( int led, bool activeLow = false );
  Att_bit& trace( Stream& stream );
  int event( int id );
  void action( int id );
};
