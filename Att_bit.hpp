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

  enum { OFF, ON };                                       // STATES
  enum { EVT_ON, EVT_OFF, EVT_TOGGLE, EVT_INPUT, ELSE };  // EVENTS
  enum { ACT_ON, ACT_OFF, ACT_INPUT };                    // ACTIONS
  enum { MODE_NULL, MODE_CALLBACK, MODE_MACHINE, MODE_TMACHINE, MODE_FACTORY };

  Att_bit& begin( bool default_state = false );
  Att_bit& onFlip( bool st, atm_cb_t callback, int idx = 0 );
  Att_bit& onFlip( bool st, Machine& machine, state_t event = 0 );
  Att_bit& onFlip( bool st, const char* label, state_t event = 0 );
  Att_bit& onFlip( bool st, TinyMachine& machine, state_t event = 0 );
  Att_bit& onInput( bool st, atm_cb_t callback, int idx = 0 );
  Att_bit& onInput( bool st, Machine& machine, state_t event = 0 );
  Att_bit& onInput( bool st, const char* label, state_t event = 0 );
  Att_bit& onInput( bool st, TinyMachine& machine, state_t event = 0 );
  Att_bit& trace( Stream& stream );
  int event( int id );
  void action( int id );
};


