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

#define ATM_CONDITION_OPERAND_MAX 4

class Att_condition : public MACHINE {
 public:
  Att_condition( void ) : MACHINE() {
#ifndef TINYMACHINE
    class_label = "CON";
#endif
  };

  state_t _last_state;
  atm_connector _connector[4];
  atm_connector _operand[ATM_CONDITION_OPERAND_MAX];
  int8_t _indicator;
  bool _indicatorActiveLow;

  enum { OFF, ON };                                       // STATES
  enum { EVT_ON, EVT_OFF, EVT_TOGGLE, EVT_INPUT, ELSE };  // EVENTS
  enum { ACT_ON, ACT_OFF, ACT_INPUT };                    // ACTIONS

  Att_condition& begin( bool default_state = false );
  Att_condition& onFlip( bool status, atm_cb_t callback, int idx = 0 );
  Att_condition& onFlip( bool status, Machine& machine, int event = 0 );
  Att_condition& onFlip( bool status, const char* label, int = 0 );
  Att_condition& onFlip( bool status, TinyMachine& machine, int event = 0 );
  Att_condition& onInput( bool status, atm_cb_t callback, int idx = 0 );
  Att_condition& onInput( bool status, Machine& machine, state_t event = 0 );
  Att_condition& onInput( bool status, const char* label, state_t event = 0 );
  Att_condition& onInput( bool status, TinyMachine& machine, state_t event = 0 );

  Att_condition& IF( Machine& machine, char relOp = '>', state_t match = 0 );
  Att_condition& IF( TinyMachine& machine, char relOp = '>', state_t match = 0 );
  Att_condition& IF( const char* label, char relOp = '>', state_t match = 0 );
  Att_condition& IF( atm_cb_t callback, char relOp = '>', state_t match = 0 );
  Att_condition& AND( Machine& machine, char relOp = '>', state_t match = 0 );
  Att_condition& AND( TinyMachine& machine, char relOp = '>', state_t match = 0 );
  Att_condition& AND( const char* label, char relOp = '>', state_t match = 0 );
  Att_condition& AND( atm_cb_t callback, char relOp = '>', state_t match = 0 );
  Att_condition& OR( Machine& machine, char relOp = '>', state_t match = 0 );
  Att_condition& OR( TinyMachine& machine, char relOp = '>', state_t match = 0 );
  Att_condition& OR( const char* label, char relOp = '>', state_t match = 0 );
  Att_condition& OR( atm_cb_t callback, char relOp = '>', state_t match = 0 );
  Att_condition& XOR( Machine& machine, char relOp = '>', state_t match = 0 );
  Att_condition& XOR( TinyMachine& machine, char relOp = '>', state_t match = 0 );
  Att_condition& XOR( const char* label, char relOp = '>', state_t match = 0 );
  Att_condition& XOR( atm_cb_t callback, char relOp = '>', state_t match = 0 );
  Att_condition& indicator( int led, bool activeLow = false );
  Att_condition& trace( Stream& stream );

 private:
  const static char relOps[];
  int event( int id );
  void action( int id );
  Att_condition& OP( char logOp, Machine& machine, char relOp, state_t match );
  Att_condition& OP( char logOp, TinyMachine& machine, char relOp, state_t match );
  Att_condition& OP( char logOp, const char* label, char relOp, state_t match );
  Att_condition& OP( char logOp, atm_cb_t callback, char relOp, state_t match );

  bool eval_one( uint8_t idx );
  bool eval_all();
};
