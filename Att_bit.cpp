#include "Att_bit.hpp"

ATM_BIT& ATM_BIT::begin( bool default_state /* = false */ ) {
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

ATM_BIT& ATM_BIT::onFlip( bool st, atm_cb_t callback, int16_t idx /* = 0 */ ) {
  _connection[st ? 0 : 1].set( callback, idx );
  return *this;
}

ATM_BIT& ATM_BIT::onFlip( bool st, Machine& machine, int16_t evt /* = 0 */ ) {
  _connection[st ? 0 : 1].set( &machine, evt );
  return *this;
}

#ifndef TINYMACHINE
ATM_BIT& ATM_BIT::onFlip( bool st, const char* label, int16_t event /* = 0 */ ) {
  _connection[st ? 0 : 1].set( label, event );
  return *this;
}
#endif

ATM_BIT& ATM_BIT::onFlip( bool st, TinyMachine& machine, int16_t event /* = 0 */ ) {
  _connection[st ? 0 : 1].set( &machine, event );
  return *this;
}

ATM_BIT& ATM_BIT::onInput( bool st, atm_cb_t callback, int16_t idx /* = 0 */ ) {
  _connection[st ? 2 : 3].set( callback, idx );
  return *this;
}

ATM_BIT& ATM_BIT::onInput( bool st, Machine& machine, state_t event /* = 0 */ ) {
  _connection[st ? 2 : 3].set( &machine, event );
  return *this;
}

#ifndef TINYMACHINE
ATM_BIT& ATM_BIT::onInput( bool st, const char* label, state_t event /* = 0 */ ) {
  _connection[st ? 2 : 3].set( label, event );
  return *this;
}
#endif

ATM_BIT& ATM_BIT::onInput( bool st, TinyMachine& machine, state_t event /* = 0 */ ) {
  _connection[st ? 2 : 3].set( &machine, event );
  return *this;
}

int ATM_BIT::event( int id ) {
  return 0;
}

void ATM_BIT::action( int id ) {
  switch ( id ) {
    case ACT_OFF:
      if ( _last_state != -1 ) _connection[_last_state == current ? 3 : 1].push( FACTORY );
      _last_state = current;
      return;
    case ACT_ON:
      if ( _last_state != -1 ) _connection[_last_state == current ? 2 : 0].push( FACTORY );
      _last_state = current;
      return;
  }
}

#ifndef TINYMACHINE
ATM_BIT& ATM_BIT::trace( Stream& stream ) {
  Machine::setTrace( &stream, atm_serial_debug::trace, "EVT_ON\0EVT_OFF\0EVT_TOGGLE\0EVT_INPUT\0ELSE\0OFF\0ON\0INPUTM" );
  return *this;
}
#endif
