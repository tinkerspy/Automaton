#include "Att_digital.hpp"

Att_digital& Att_digital::begin( int attached_pin, int debounce /* = 20 */, bool activeLow /* = false */, bool pullUp /* = false */ ) {
  // clang-format off
  const static STATE_TYPE state_table[] PROGMEM = {
    /*              ON_ENTER    ON_LOOP      ON_EXIT  EVT_TIMER   EVT_HIGH  EVT_LOW   ELSE */
    /* IDLE    */         -1,        -1,          -1,        -1,     WAITH,      -1,    -1,
    /* WAITH   */         -1,        -1,          -1,     VHIGH,        -1,    IDLE,    -1,
    /* VHIGH   */   ACT_HIGH,        -1,          -1,        -1,        -1,   WAITL,    -1, 
    /* WAITL   */         -1,        -1,          -1,      VLOW,     VHIGH,      -1,    -1,
    /* VLOW    */    ACT_LOW,        -1,          -1,        -1,        -1,      -1,  IDLE,
  };
  // clang-format on
  MACHINE::begin( state_table, ELSE );
  pin = attached_pin;
  _activeLow = activeLow;
  timer.set( debounce );
  pinMode( pin, pullUp ? INPUT_PULLUP : INPUT );
  return *this;
}

int Att_digital::state( void ) {
  return ( current == VHIGH || current == WAITL );
}

Att_digital& Att_digital::onFlip( bool st, atm_cb_t callback, int idx /* = 0 */ ) {
  _connection[st ? 1 : 0].set( callback, idx );
  return *this;
}

Att_digital& Att_digital::onFlip( bool st, Machine& machine, int event /* = 0 */ ) {
  _connection[st ? 1 : 0].set( &machine, event );
  return *this;
}

Att_digital& Att_digital::onFlip( bool st, TinyMachine& machine, int event /* = 0 */ ) {
  _connection[st ? 1 : 0].set( &machine, event );
  return *this;
}

#ifndef TINYMACHINE
Att_digital& Att_digital::onFlip( bool st, const char* label, int event /* = 0 */ ) {
  _connection[st ? 1 : 0].set( label, event );
  return *this;
}
#endif

int Att_digital::event( int id ) {
  switch ( id ) {
    case EVT_TIMER:
      return timer.expired( this );
    case EVT_HIGH:
      return ( !digitalRead( pin ) != !_activeLow );  // XOR
    case EVT_LOW:
      return !( !digitalRead( pin ) != !_activeLow );
  }
  return 0;
}

void Att_digital::action( int id ) {
  switch ( id ) {
    case ACT_HIGH:
      _connection[1].push( FACTORY );
      return;
    case ACT_LOW:
      _connection[0].push( FACTORY );
      return;
  }
}

Att_digital& Att_digital::trace( Stream& stream ) {
#ifndef TINYMACHINE
  setTrace( &stream, atm_serial_debug::trace, "EVT_TIMER\0EVT_HIGH\0EVT_LOW\0ELSE\0IDLE\0WAITH\0VHIGH\0WAITL\0VLOW" );
#endif
  return *this;
}
