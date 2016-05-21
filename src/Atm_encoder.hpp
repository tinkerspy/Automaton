#pragma once

#include <Automaton.h>

class Atm_encoder : public Machine {
 public:
  Atm_encoder( void ) : Machine(){};
  short _pin1, _pin2;
  const static char _enc_states[];
  uint8_t _enc_bits;
  uint8_t _enc_counter;
  int8_t _enc_direction;
  int _divider;
  int _value, _min, _max;
  bool _wrap;
  atm_connector _onup;
  atm_connector _ondown;

  enum { IDLE, UP, DOWN };                // STATES
  enum { EVT_UP, EVT_DOWN, ELSE };        // EVENTS
  enum { ACT_SAMPLE, ACT_UP, ACT_DOWN };  // ACTIONS

  Atm_encoder& begin( int pin1, int pin2, int divider = 1 );
  Atm_encoder& trace( Stream& stream );
  Atm_encoder& onChange( Machine& machine, int event = 0 );
  Atm_encoder& onChange( atm_cb_push_t callback, int idx = 0 );
  Atm_encoder& onUp( Machine& machine, int event = 0 );
  Atm_encoder& onUp( atm_cb_push_t callback, int idx = 0 );
  Atm_encoder& onDown( Machine& machine, int event = 0 );
  Atm_encoder& onDown( atm_cb_push_t callback, int idx = 0 );
  int state( void );
  Atm_encoder& range( int min, int max, bool wrap = false );
  Atm_encoder& set( int value );

 private:
  bool count( int direction );
  int event( int id );
  void action( int id );
};
