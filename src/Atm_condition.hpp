#pragma once

#include <Automaton.h>

#undef TINYMACHINE
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

#define ATM_CONDITION_OPERAND_MAX 4

class Atm_condition : public MACHINE {
 public:
  Atm_condition( void ) : MACHINE() {
#ifndef TINYMACHINE
    class_label = "CON";
#endif
  };

  state_t _last_state;
  atm_connector _connector[4];
  atm_connector _operand[ATM_CONDITION_OPERAND_MAX];

  enum { OFF, ON };                                       // STATES
  enum { EVT_ON, EVT_OFF, EVT_TOGGLE, EVT_INPUT, ELSE };  // EVENTS
  enum { ACT_ON, ACT_OFF, ACT_INPUT };                    // ACTIONS

  Atm_condition& begin( bool default_state = false );
  Atm_condition& onFlip( bool st, atm_cb_t callback, int idx = 0 );
  Atm_condition& onFlip( bool st, Machine& machine, int event = 0 );
  Atm_condition& onFlip( bool st, const char* label, int = 0 );
  Atm_condition& onFlip( bool st, TinyMachine& machine, int event = 0 );
  Atm_condition& onInput( bool st, atm_cb_t callback, int idx = 0 );
  Atm_condition& onInput( bool st, Machine& machine, state_t event = 0 );
  Atm_condition& onInput( bool st, const char* label, state_t event = 0 );
  Atm_condition& onInput( bool st, TinyMachine& machine, state_t event = 0 );

  Atm_condition& IF( Machine& machine, char relOp = '>', state_t match = 0 );
  Atm_condition& IF( TinyMachine& machine, char relOp = '>', state_t match = 0 );
  Atm_condition& IF( const char* label, char relOp = '>', state_t match = 0 );
  Atm_condition& IF( atm_cb_t callback, char relOp = '>', state_t match = 0 );
  Atm_condition& AND( Machine& machine, char relOp = '>', state_t match = 0 );
  Atm_condition& AND( TinyMachine& machine, char relOp = '>', state_t match = 0 );
  Atm_condition& AND( const char* label, char relOp = '>', state_t match = 0 );
  Atm_condition& AND( atm_cb_t callback, char relOp = '>', state_t match = 0 );
  Atm_condition& OR( Machine& machine, char relOp = '>', state_t match = 0 );
  Atm_condition& OR( TinyMachine& machine, char relOp = '>', state_t match = 0 );
  Atm_condition& OR( const char* label, char relOp = '>', state_t match = 0 );
  Atm_condition& OR( atm_cb_t callback, char relOp = '>', state_t match = 0 );
  Atm_condition& XOR( Machine& machine, char relOp = '>', state_t match = 0 );
  Atm_condition& XOR( TinyMachine& machine, char relOp = '>', state_t match = 0 );
  Atm_condition& XOR( const char* label, char relOp = '>', state_t match = 0 );
  Atm_condition& XOR( atm_cb_t callback, char relOp = '>', state_t match = 0 );
  Atm_condition& trace( Stream& stream );

 private:
  const static char relOps[];
  int event( int id );
  void action( int id );
  Atm_condition& OP( char logOp, Machine& machine, char relOp, state_t match );
  Atm_condition& OP( char logOp, TinyMachine& machine, char relOp, state_t match );
  Atm_condition& OP( char logOp, const char* label, char relOp, state_t match );
  Atm_condition& OP( char logOp, atm_cb_t callback, char relOp, state_t match );

  bool eval_one( uint8_t idx );
  bool eval_all();
};
