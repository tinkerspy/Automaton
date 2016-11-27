#pragma once

#include <Automaton.h>

class Atm_bit : public Machine {
 public:
  enum { OFF, ON, REFR_ON, REFR_OFF };                                       // STATES
  enum { EVT_ON, EVT_OFF, EVT_TOGGLE, EVT_INPUT, EVT_REFRESH, ELSE };  // EVENTS

  Atm_bit( void ) : Machine(){};
  Atm_bit& begin( bool initialState = false );
  Atm_bit& onChange( atm_cb_push_t callback, int idx = 0 );
  Atm_bit& onChange( Machine& machine, int event = 0 );
  Atm_bit& onChange( bool status, atm_cb_push_t callback, int idx = 0 );
  Atm_bit& onChange( bool status, Machine& machine, int event = 0 );
  Atm_bit& onInput( bool status, atm_cb_push_t callback, int idx = 0 );
  Atm_bit& onInput( bool status, Machine& machine, int event = 0 );
  Atm_bit& led( int led, bool activeLow = false );
  Atm_bit& on( void );
  Atm_bit& off( void );
  Atm_bit& toggle( void );
  Atm_bit& input( void );
  Atm_bit& refresh( void );
  Atm_bit& trace( Stream& stream );

 private:
  enum { ENT_ON, ENT_OFF, ENT_REFR_ON, ENT_REFR_OFF };                                                              // ACTIONS
  enum { ON_CHANGE_FALSE, ON_CHANGE_TRUE, ON_INPUT_FALSE, ON_INPUT_TRUE, _CONN_SIZE_ };  // CONNECTORS
  state_t last_state;
  atm_connector connector[_CONN_SIZE_];
  int8_t indicator;
  bool indicatorActiveLow;

  int event( int id );
  void action( int id );
};
