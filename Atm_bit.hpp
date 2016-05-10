#pragma once

#include <Automaton.h>

#undef TINYMACHINE

#ifdef TINYMACHINE
#define ATM_BIT Att_bit
#define MACHINE TinyMachine
#define FACTORY 0
#define STATE_TYPE tiny_state_t
#else
#define ATM_BIT Atm_bit
#define MACHINE Machine
#define FACTORY factory
#define STATE_TYPE state_t
#endif

class ATM_BIT : public MACHINE {
 public:
  ATM_BIT( void ) : MACHINE() {
#ifndef TINYMACHINE      
    class_label = "BIT";
#endif    
  };

  state_t _last_state;
  atm_connector _connection[4];

  enum { OFF, ON };                                       // STATES
  enum { EVT_ON, EVT_OFF, EVT_TOGGLE, EVT_INPUT, ELSE };  // EVENTS
  enum { ACT_ON, ACT_OFF, ACT_INPUT };                    // ACTIONS
  enum { MODE_NULL, MODE_CALLBACK, MODE_MACHINE, MODE_TMACHINE, MODE_FACTORY };

  ATM_BIT& begin( bool default_state = false );
  ATM_BIT& onFlip( bool st, atm_cb_t callback, int16_t idx = 0 );
  ATM_BIT& onFlip( bool st, Machine& machine, state_t event = 0 );
  ATM_BIT& onFlip( bool st, const char* label, state_t event = 0 );
  ATM_BIT& onFlip( bool st, TinyMachine& machine, state_t event = 0 );
  ATM_BIT& onInput( bool st, atm_cb_t callback, int16_t idx = 0 );
  ATM_BIT& onInput( bool st, Machine& machine, state_t event = 0 );
  ATM_BIT& onInput( bool st, const char* label, state_t event = 0 );
  ATM_BIT& onInput( bool st, TinyMachine& machine, state_t event = 0 );
  ATM_BIT& trace( Stream& stream );
  int event( int id );
  void action( int id );
};


