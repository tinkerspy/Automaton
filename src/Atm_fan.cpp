#include "Atm_fan.hpp"

Atm_fan& Atm_fan::begin() {
  // clang-format off
  const static STATE_TYPE state_table[] PROGMEM = {
    /*            ON_ENTER    ON_LOOP  ON_EXIT  EVT_INPUT  ELSE */
    /* IDLE    */       -1, ATM_SLEEP,      -1,      SEND,   -1,
    /* SEND    */ ACT_SEND,        -1,      -1,        -1, IDLE,
  };
  // clang-format on
  MACHINE::begin( state_table, ELSE );
  return *this;
}

Atm_fan& Atm_fan::onInput( atm_cb_t callback, int idx ) {
  for ( uint8_t idx = 0; idx < ATM_MULTIPLIER_COMMS_MAX; idx++ ) {
    if ( _connector[idx].mode() == atm_connector::MODE_NULL ) {
      _connector[idx].set( callback, idx );
      break;
    }
  }
  return *this;
}

Atm_fan& Atm_fan::onInput( Machine& machine, state_t event /* = 0 */ ) {
  for ( uint8_t idx = 0; idx < ATM_MULTIPLIER_COMMS_MAX; idx++ ) {
    if ( _connector[idx].mode() == atm_connector::MODE_NULL ) {
      _connector[idx].set( &machine, event );
      break;
    }
  }
  return *this;
}

#ifndef TINYMACHINE
Atm_fan& Atm_fan::onInput( const char* label, state_t event /* = 0 */ ) {
  for ( uint8_t idx = 0; idx < ATM_MULTIPLIER_COMMS_MAX; idx++ ) {
    if ( _connector[idx].mode() == atm_connector::MODE_NULL ) {
      _connector[idx].set( label, event );
      break;
    }
  }
  return *this;
}
#endif

Atm_fan& Atm_fan::onInput( TinyMachine& machine, state_t event /* = 0 */ ) {
  for ( uint8_t i = 0; i < ATM_MULTIPLIER_COMMS_MAX; i++ ) {
    if ( _connector[i].mode() == atm_connector::MODE_NULL ) {
      _connector[i].set( &machine, event );
      break;
    }
  }
  return *this;
}

int Atm_fan::event( int id ) {
  return 0;
}

void Atm_fan::action( int id ) {
  switch ( id ) {
    case ACT_SEND:
      for ( uint8_t i = 0; i < ATM_MULTIPLIER_COMMS_MAX; i++ ) {
        if ( _connector[i].mode() != atm_connector::MODE_NULL ) {
          _connector[i].push( FACTORY );
        }
      }
      return;
  }
}

Atm_fan& Atm_fan::trace( Stream& stream ) {
#ifndef TINYMACHINE
  Machine::setTrace( &stream, atm_serial_debug::trace, "EVT_INPUT\0ELSE\0IDLE\0SEND" );
#endif
  return *this;
}
