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

#define ATM_CONDITION_OPERAND_MAX 4

class Atm_condition : public MACHINE {
 public:
  Atm_condition( void ) : MACHINE() {};

  state_t _last_state;
  atm_connector _connector[4];
  atm_connector _operand[ATM_CONDITION_OPERAND_MAX];
  int8_t _indicator;
  bool _indicatorActiveLow;

  enum { OFF, ON };                                       // STATES
  enum { EVT_ON, EVT_OFF, EVT_TOGGLE, EVT_INPUT, ELSE };  // EVENTS
  enum { ACT_ON, ACT_OFF, ACT_INPUT };                    // ACTIONS

  Atm_condition& begin( bool initialState = false );
  Atm_condition& onFlip( bool status, atm_cb_t callback, int idx = 0 );
  Atm_condition& onFlip( bool status, Machine& machine, int event = 0 );
  Atm_condition& onInput( bool status, atm_cb_t callback, int idx = 0 );
  Atm_condition& onInput( bool status, Machine& machine, state_t event = 0 );

  Atm_condition& IF( Machine& machine, char relOp = '>', state_t match = 0 );
  Atm_condition& IF( atm_cb_t callback, char relOp = '>', state_t match = 0 );
  Atm_condition& AND( Machine& machine, char relOp = '>', state_t match = 0 );
  Atm_condition& AND( atm_cb_t callback, char relOp = '>', state_t match = 0 );
  Atm_condition& OR( Machine& machine, char relOp = '>', state_t match = 0 );
  Atm_condition& OR( atm_cb_t callback, char relOp = '>', state_t match = 0 );
  Atm_condition& XOR( Machine& machine, char relOp = '>', state_t match = 0 );
  Atm_condition& XOR( atm_cb_t callback, char relOp = '>', state_t match = 0 );
  Atm_condition& indicator( int led, bool activeLow = false );
  Atm_condition& trace( Stream& stream );

 private:
  const static char relOps[];
  int event( int id );
  void action( int id );
  Atm_condition& OP( char logOp, Machine& machine, char relOp, state_t match );
  Atm_condition& OP( char logOp, atm_cb_t callback, char relOp, state_t match );

  bool eval_one( uint8_t idx );
  bool eval_all();
};
