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

#define ATM_MULTIPLIER_COMMS_MAX 4

class Atm_fan : public MACHINE {
 public:
  Atm_fan( void ) : MACHINE() {};

  atm_connector _connector[ATM_MULTIPLIER_COMMS_MAX];

  enum { IDLE, SEND };       // STATES
  enum { EVT_INPUT, ELSE };  // EVENTS
  enum { ACT_SEND };         // ACTIONS

  Atm_fan& begin();
  Atm_fan& trace( Stream& stream );
  Atm_fan& onInput( atm_cb_t callback, int idx = 0 );
  Atm_fan& onInput( Machine& machine, state_t event = 0 );

 private:
  int event( int id );
  void action( int id );
};
