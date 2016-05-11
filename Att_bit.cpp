#include "Att_bit.hpp"

Att_bit& Att_bit::begin( bool default_state /* = false */ ) {
  // clang-format off
  const static STATE_TYPE state_table[] PROGMEM = {
    /*              ON_ENTER    ON_LOOP  ON_EXIT  EVT_ON  EVT_OFF  EVT_TOGGLE EVT_INPUT ELSE */
    /* OFF     */    ACT_OFF, ATM_SLEEP,      -1,     ON,      -1,         ON,      OFF,  -1,
    /* ON      */     ACT_ON, ATM_SLEEP,      -1,     -1,     OFF,        OFF,       ON,  -1,
  };
  // clang-format on
  MACHINE::begin( state_table, ELSE );
  _last_state = -1;
  state( default_state ? ON : OFF );
  cycle();
  return *this;
}

Att_bit& Att_bit::onFlip( bool st, atm_cb_t callback, int idx /* = 0 */ ) {
  _connector[st ? 0 : 1].set( callback, idx );
  return *this;
}

Att_bit& Att_bit::onFlip( bool st, Machine& machine, int evt /* = 0 */ ) {
  _connector[st ? 0 : 1].set( &machine, evt );
  return *this;
}

#ifndef TINYMACHINE
Att_bit& Att_bit::onFlip( bool st, const char* label, int event /* = 0 */ ) {
  _connector[st ? 0 : 1].set( label, event );
  return *this;
}
#endif

Att_bit& Att_bit::onFlip( bool st, TinyMachine& machine, int event /* = 0 */ ) {
  _connector[st ? 0 : 1].set( &machine, event );
  return *this;
}

Att_bit& Att_bit::onInput( bool st, atm_cb_t callback, int idx /* = 0 */ ) {
  _connector[st ? 2 : 3].set( callback, idx );
  return *this;
}

Att_bit& Att_bit::onInput( bool st, Machine& machine, state_t event /* = 0 */ ) {
  _connector[st ? 2 : 3].set( &machine, event );
  return *this;
}

#ifndef TINYMACHINE
Att_bit& Att_bit::onInput( bool st, const char* label, state_t event /* = 0 */ ) {
  _connector[st ? 2 : 3].set( label, event );
  return *this;
}
#endif

Att_bit& Att_bit::onInput( bool st, TinyMachine& machine, state_t event /* = 0 */ ) {
  _connector[st ? 2 : 3].set( &machine, event );
  return *this;
}

int Att_bit::event( int id ) {
  return 0;
}

void Att_bit::action( int id ) {
  switch ( id ) {
    case ACT_OFF:
      if ( _last_state != -1 ) _connector[_last_state == current ? 3 : 1].push( FACTORY );
      _last_state = current;
      return;
    case ACT_ON:
      if ( _last_state != -1 ) _connector[_last_state == current ? 2 : 0].push( FACTORY );
      _last_state = current;
      return;
  }
}

Att_bit& Att_bit::trace( Stream& stream ) {
#ifndef TINYMACHINE
  Machine::setTrace( &stream, atm_serial_debug::trace, "EVT_ON\0EVT_OFF\0EVT_TOGGLE\0EVT_INPUT\0ELSE\0OFF\0ON\0INPUTM" );
#endif
  return *this;
}
