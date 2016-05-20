#pragma once

#include <Automaton.h>

#define ATM_CONDITION_OPERAND_MAX 4

class Atm_controller : public Machine {
 public:
  Atm_controller( void ) : Machine(){};

  state_t _last_state;
  atm_connector _connector[4];
  atm_connector _operand[ATM_CONDITION_OPERAND_MAX];
  int8_t _indicator;
  bool _indicatorActiveLow;

  enum { OFF, ON };                                       // STATES
  enum { EVT_ON, EVT_OFF, EVT_TOGGLE, EVT_INPUT, ELSE };  // EVENTS
  enum { ACT_ON, ACT_OFF, ACT_INPUT };                    // ACTIONS

  Atm_controller& begin( bool initialState = false );
  Atm_controller& onFlip( bool status, atm_cb_push_t callback, int idx = 0 );
  Atm_controller& onFlip( bool status, Machine& machine, int event = 0 );
  Atm_controller& onInput( bool status, atm_cb_push_t callback, int idx = 0 );
  Atm_controller& onInput( bool status, Machine& machine, int event = 0 );

  Atm_controller& IF( Machine& machine, char relOp = '>', int match = 0 );
  Atm_controller& IF( atm_cb_pull_t callback, char relOp = '>', int match = 0 );
  Atm_controller& AND( Machine& machine, char relOp = '>', int match = 0 );
  Atm_controller& AND( atm_cb_pull_t callback, char relOp = '>', int match = 0 );
  Atm_controller& OR( Machine& machine, char relOp = '>', int match = 0 );
  Atm_controller& OR( atm_cb_pull_t callback, char relOp = '>', int match = 0 );
  Atm_controller& XOR( Machine& machine, char relOp = '>', int match = 0 );
  Atm_controller& XOR( atm_cb_pull_t callback, char relOp = '>', int match = 0 );
  Atm_controller& indicator( int led, bool activeLow = false );
  Atm_controller& trace( Stream& stream );

 private:
  const static char relOps[];
  int event( int id );
  void action( int id );
  Atm_controller& OP( char logOp, Machine& machine, char relOp, int match );
  Atm_controller& OP( char logOp, atm_cb_pull_t callback, char relOp, int match );

  bool eval_one( atm_connector& connector );
  bool eval_all();
};
