#include "Atm_digital.hpp"

Atm_digital& Atm_digital::begin( int pin, int debounce /* = 20 */, bool activeLow /* = false */, bool pullUp /* = false */ ) {
  // clang-format off
  const static state_t state_table[] PROGMEM = {
    /*              ON_ENTER    ON_LOOP      ON_EXIT  EVT_TIMER   EVT_HIGH  EVT_LOW   ELSE */
    /* IDLE    */         -1,        -1,          -1,        -1,     WAITH,      -1,    -1,
    /* WAITH   */         -1,        -1,          -1,     VHIGH,        -1,    IDLE,    -1,
    /* VHIGH   */   ENT_HIGH,        -1,          -1,        -1,        -1,   WAITL,    -1, 
    /* WAITL   */         -1,        -1,          -1,      VLOW,     VHIGH,      -1,    -1,
    /* VLOW    */    ENT_LOW,        -1,          -1,        -1,        -1,      -1,  IDLE,
  };
  // clang-format on
  Machine::begin( state_table, ELSE );
  this->pin = pin;
  this->activeLow = activeLow;
  timer.set( debounce );
  indicator = -1;
  pinMode( pin, pullUp ? INPUT_PULLUP : INPUT );
  return *this;
}

int Atm_digital::event( int id ) {
  switch ( id ) {
    case EVT_TIMER:
      return timer.expired( this );
    case EVT_HIGH:
      return ( !digitalRead( pin ) != !activeLow );  // XOR
    case EVT_LOW:
      return !( !digitalRead( pin ) != !activeLow );
  }
  return 0;
}

void Atm_digital::action( int id ) {
  switch ( id ) {
    case ENT_HIGH:
      connection[ON_CHANGE_TRUE].push( state() );
      if ( indicator > -1 ) digitalWrite( indicator, !HIGH != !indicatorActiveLow );
      return;
    case ENT_LOW:
      connection[ON_CHANGE_FALSE].push( state() );
      if ( indicator > -1 ) digitalWrite( indicator, !LOW != !indicatorActiveLow );
      return;
  }
}

int Atm_digital::state( void ) {
  return ( current == VHIGH || current == WAITL );
}

Atm_digital& Atm_digital::led( int led, bool activeLow /* = false */ ) {
  indicator = led;
  indicatorActiveLow = activeLow;
  pinMode( indicator, OUTPUT );
  return *this;
}

Atm_digital& Atm_digital::onChange( bool status, atm_cb_push_t callback, int idx /* = 0 */ ) {
  connection[status ? ON_CHANGE_TRUE : ON_CHANGE_FALSE].set( callback, idx );
  return *this;
}

Atm_digital& Atm_digital::onChange( bool status, Machine& machine, int event /* = 0 */ ) {
  connection[status ? ON_CHANGE_TRUE : ON_CHANGE_FALSE].set( &machine, event );
  return *this;
}

Atm_digital& Atm_digital::onChange( atm_cb_push_t callback, int idx /* = 0 */ ) {
  connection[ON_CHANGE_FALSE].set( callback, idx );
  connection[ON_CHANGE_TRUE].set( callback, idx );
  return *this;
}

Atm_digital& Atm_digital::onChange( Machine& machine, int event /* = 0 */ ) {
  connection[ON_CHANGE_FALSE].set( &machine, event );
  connection[ON_CHANGE_TRUE].set( &machine, event );
  return *this;
}

Atm_digital& Atm_digital::trace( Stream& stream ) {
  setTrace( &stream, atm_serial_debug::trace, "DIGITAL\0EVT_TIMER\0EVT_HIGH\0EVT_LOW\0ELSE\0IDLE\0WAITH\0VHIGH\0WAITL\0VLOW" );
  return *this;
}
