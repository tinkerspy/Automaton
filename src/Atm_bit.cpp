#include "Atm_bit.hpp"

Atm_bit& Atm_bit::begin( bool initialState /* = false */ ) {
  // clang-format off
  const static state_t state_table[] PROGMEM = {
    /*              ON_ENTER    ON_LOOP  ON_EXIT  EVT_ON  EVT_OFF  EVT_TOGGLE EVT_INPUT ELSE */
    /* OFF     */    ACT_OFF, ATM_SLEEP,      -1,     ON,      -1,         ON,      OFF,  -1,
    /* ON      */     ACT_ON, ATM_SLEEP,      -1,     -1,     OFF,        OFF,       ON,  -1,
  };
  // clang-format on
  Machine::begin( state_table, ELSE );
  _last_state = -1;
  state( initialState ? ON : OFF );
  _indicator = -1;
  cycle();
  return *this;
}

int Atm_bit::event( int id ) {
  return 0;
}

void Atm_bit::action( int id ) {
  switch ( id ) {
    case ACT_OFF:
      if ( _last_state != -1 ) _connector[_last_state == current ? 3 : 1].push();
      if ( _indicator > -1 ) digitalWrite( _indicator, !LOW != !_indicatorActiveLow );
      _last_state = current;
      return;
    case ACT_ON:
      if ( _last_state != -1 ) _connector[_last_state == current ? 2 : 0].push();
      if ( _indicator > -1 ) digitalWrite( _indicator, !HIGH != !_indicatorActiveLow );
      _last_state = current;
      return;
  }
}

Atm_bit& Atm_bit::led( int led, bool activeLow /* = false */ ) {
  _indicator = led;
  _indicatorActiveLow = activeLow;
  pinMode( _indicator, OUTPUT );
  return *this;
}

Atm_bit& Atm_bit::onChange( atm_cb_push_t callback, int idx /* = 0 */ ) {
  _connector[0].set( callback, idx );
  _connector[1].set( callback, idx );
  return *this;
}

Atm_bit& Atm_bit::onChange( Machine& machine, int event /* = 0 */ ) {
  _connector[0].set( &machine, event );
  _connector[1].set( &machine, event );
  return *this;
}

Atm_bit& Atm_bit::onChange( bool status, atm_cb_push_t callback, int idx /* = 0 */ ) {
  _connector[status ? 0 : 1].set( callback, idx );
  return *this;
}

Atm_bit& Atm_bit::onChange( bool status, Machine& machine, int event /* = 0 */ ) {
  _connector[status ? 0 : 1].set( &machine, event );
  return *this;
}

Atm_bit& Atm_bit::onInput( bool status, atm_cb_push_t callback, int idx /* = 0 */ ) {
  _connector[status ? 2 : 3].set( callback, idx );
  return *this;
}

Atm_bit& Atm_bit::onInput( bool status, Machine& machine, int event /* = 0 */ ) {
  _connector[status ? 2 : 3].set( &machine, event );
  return *this;
}
Atm_bit& Atm_bit::trace( Stream& stream ) {
  Machine::setTrace( &stream, atm_serial_debug::trace, "BIT\0EVT_ON\0EVT_OFF\0EVT_TOGGLE\0EVT_INPUT\0ELSE\0OFF\0ON\0INPUTM" );
  return *this;
}
