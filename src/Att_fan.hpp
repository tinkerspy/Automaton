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

#define ATM_MULTIPLIER_COMMS_MAX 4

class Att_fan : public MACHINE {
 public:
  Att_fan( void ) : MACHINE() {
#ifndef TINYMACHINE
    class_label = "MUL";
#endif
  };

  atm_connector _connector[ATM_MULTIPLIER_COMMS_MAX];

  enum { IDLE, SEND };       // STATES
  enum { EVT_INPUT, ELSE };  // EVENTS
  enum { ACT_SEND };         // ACTIONS

  Att_fan& begin();
  Att_fan& trace( Stream& stream );
  Att_fan& onInput( atm_cb_t callback, int idx = 0 );
  Att_fan& onInput( Machine& machine, state_t event = 0 );
  Att_fan& onInput( const char* label, state_t event = 0 );
  Att_fan& onInput( TinyMachine& machine, state_t event = 0 );

 private:
  int event( int id );
  void action( int id );
};
