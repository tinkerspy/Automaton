#pragma once

#include <Automaton.h>

#define ATM_CONDITION_OPERAND_MAX 4

class Atm_controller : public Machine {
 public:
  enum { OFF, ON };                           // STATES
  enum { EVT_ON, EVT_OFF, EVT_INPUT, ELSE };  // EVENTS

  Atm_controller( void ) : Machine(){};
  Atm_controller& begin( bool initialState = false );
  Atm_controller& onChange( bool status, atm_cb_push_t callback, int idx = 0 );
  Atm_controller& onChange( bool status, Machine& machine, int event = 0 );
  Atm_controller& onChange( atm_cb_push_t callback, int idx = 0 );
  Atm_controller& onChange( Machine& machine, int event = 0 );
  Atm_controller& onInput( bool status, atm_cb_push_t callback, int idx = 0 );
  Atm_controller& onInput( bool status, Machine& machine, int event = 0 );
  Atm_controller& IF( Machine& machine, char relOp = '>', int match = 0 );
  Atm_controller& IF( atm_cb_pull_t callback, int idx = 0 );
  Atm_controller& AND( Machine& machine, char relOp = '>', int match = 0 );
  Atm_controller& AND( atm_cb_pull_t callback, int idx = 0 );
  Atm_controller& OR( Machine& machine, char relOp = '>', int match = 0 );
  Atm_controller& OR( atm_cb_pull_t callback, int idx = 0 );
  Atm_controller& XOR( Machine& machine, char relOp = '>', int match = 0 );
  Atm_controller& XOR( atm_cb_pull_t callback, int idx = 0 );
  Atm_controller& led( int led, bool activeLow = false );
  Atm_controller& trace( Stream& stream );

 private:
  enum { ENT_ON, ENT_OFF };                                                              // ACTIONS
  enum { ON_CHANGE_FALSE, ON_CHANGE_TRUE, ON_INPUT_FALSE, ON_INPUT_TRUE, _CONN_SIZE_ };  // CONNECTORS
  state_t last_state;
  atm_connector connector[_CONN_SIZE_];
  atm_connector operand[ATM_CONDITION_OPERAND_MAX];
  int8_t indicator;
  bool indicatorActiveLow;
  const static char relOps[];

  int event( int id );
  void action( int id );
  Atm_controller& OP( char logOp, Machine& machine, char relOp, int match );
  Atm_controller& OP( char logOp, atm_cb_pull_t callback, int idx );
  bool eval_one( atm_connector& connector );
  bool eval_all();
};
