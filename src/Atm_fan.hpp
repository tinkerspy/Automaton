#pragma once

#include <Automaton.h>

#define ATM_MULTIPLIER_COMMS_MAX 4

class Atm_fan : public Machine {
 public:
  Atm_fan( void ) : Machine(){};

  atm_connector _connector[ATM_MULTIPLIER_COMMS_MAX];

  enum { IDLE, SEND };       // STATES
  enum { EVT_INPUT, ELSE };  // EVENTS
  enum { ACT_SEND };         // ACTIONS

  Atm_fan& begin();
  Atm_fan& trace( Stream& stream );
  Atm_fan& onInput( atm_cb_t callback, int idx = 0 );
  Atm_fan& onInput( Machine& machine, int event = 0 );

 private:
  int event( int id );
  void action( int id );
};
