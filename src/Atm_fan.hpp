#pragma once

#include <Automaton.h>

#define ATM_MULTIPLIER_COMMS_MAX 4

class Atm_fan : public Machine {
 public:
  enum { IDLE, SEND };       // STATES
  enum { EVT_INPUT, ELSE };  // EVENTS

  Atm_fan( void ) : Machine(){};
  Atm_fan& begin();
  Atm_fan& trace( Stream& stream );
  Atm_fan& onInput( atm_cb_push_t callback, int idx = 0 );
  Atm_fan& onInput( Machine& machine, int event = 0 );

 private:
  enum { ENT_SEND };  // ACTIONS
  atm_connector connector[ATM_MULTIPLIER_COMMS_MAX];
  int event( int id );
  void action( int id );
};
