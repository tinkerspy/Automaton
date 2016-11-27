#include "Atm_bit.hpp"

Atm_bit& Atm_bit::begin( bool initialState /* = false */ ) {
  // clang-format off
  const static state_t state_table[] PROGMEM = {
    /*                ON_ENTER    ON_LOOP  ON_EXIT  EVT_ON  EVT_OFF  EVT_TOGGLE EVT_INPUT EVT_REFRESH ELSE */
    /* OFF     */      ENT_OFF, ATM_SLEEP,      -1,     ON,      -1,         ON,      OFF,   REFR_OFF,  -1,
    /* ON      */       ENT_ON, ATM_SLEEP,      -1,     -1,     OFF,        OFF,       ON,    REFR_ON,  -1,
    /* REFR_ON */  ENT_REFR_ON,        -1,      -1,     -1,      -1,         -1,       -1,         -1,  ON,
    /* REFR_OFF*/ ENT_REFR_OFF,        -1,      -1,     -1,      -1,         -1,       -1,         -1, OFF,
  };
  // clang-format on
  Machine::begin( state_table, ELSE );
  last_state = -1;
  state( initialState ? ON : OFF );
  indicator = -1;
  cycle();
  return *this;
}

int Atm_bit::event( int id ) {
  return 0;
}

void Atm_bit::action( int id ) {
  switch ( id ) {
    case ENT_OFF:
      if ( last_state != -1 ) connector[last_state == current ? ON_INPUT_FALSE : ON_CHANGE_FALSE].push( state() );
      if ( indicator > -1 ) digitalWrite( indicator, !LOW != !indicatorActiveLow );
      last_state = current;
      return;
    case ENT_ON:
      if ( last_state != -1 ) connector[last_state == current ? ON_INPUT_TRUE : ON_CHANGE_TRUE].push( state() );
      if ( indicator > -1 ) digitalWrite( indicator, !HIGH != !indicatorActiveLow );
      last_state = current;
      return;
    case ENT_REFR_ON:
      connector[ON_CHANGE_TRUE].push( ON );
      last_state = -1;
      return;      
    case ENT_REFR_OFF:
      connector[ON_CHANGE_FALSE].push( OFF );
      last_state = -1;
      return;      
  }
}

Atm_bit& Atm_bit::on( void ) {
  trigger( EVT_ON );
  return *this;
}

Atm_bit& Atm_bit::off( void ) {
  trigger( EVT_OFF );
  return *this;
}

Atm_bit& Atm_bit::toggle( void ) {
  trigger( EVT_TOGGLE );
  return *this;
}

Atm_bit& Atm_bit::input( void ) {
  trigger( EVT_INPUT );
  return *this;
}

Atm_bit& Atm_bit::refresh( void ) {
  trigger( EVT_REFRESH );
  return *this;
}

Atm_bit& Atm_bit::led( int led, bool activeLow /* = false */ ) {
  indicator = led;
  indicatorActiveLow = activeLow;
  pinMode( indicator, OUTPUT );
  return *this;
}

Atm_bit& Atm_bit::onChange( atm_cb_push_t callback, int idx /* = 0 */ ) {
  connector[ON_CHANGE_FALSE].set( callback, idx );
  connector[ON_CHANGE_TRUE].set( callback, idx );
  return *this;
}

Atm_bit& Atm_bit::onChange( Machine& machine, int event /* = 0 */ ) {
  connector[ON_CHANGE_FALSE].set( &machine, event );
  connector[ON_CHANGE_TRUE].set( &machine, event );
  return *this;
}

Atm_bit& Atm_bit::onChange( bool status, atm_cb_push_t callback, int idx /* = 0 */ ) {
  connector[status ? ON_CHANGE_TRUE : ON_CHANGE_FALSE].set( callback, idx );
  return *this;
}

Atm_bit& Atm_bit::onChange( bool status, Machine& machine, int event /* = 0 */ ) {
  connector[status ? ON_CHANGE_TRUE : ON_CHANGE_FALSE].set( &machine, event );
  return *this;
}

Atm_bit& Atm_bit::onInput( bool status, atm_cb_push_t callback, int idx /* = 0 */ ) {
  connector[status ? ON_INPUT_TRUE : ON_INPUT_FALSE].set( callback, idx );
  return *this;
}

Atm_bit& Atm_bit::onInput( bool status, Machine& machine, int event /* = 0 */ ) {
  connector[status ? ON_INPUT_TRUE : ON_INPUT_FALSE].set( &machine, event );
  return *this;
}

Atm_bit& Atm_bit::trace( Stream& stream ) {
  Machine::setTrace( &stream, atm_serial_debug::trace, "BIT\0EVT_ON\0EVT_OFF\0EVT_TOGGLE\0EVT_INPUT\0EVT_REFRESH\0ELSE\0OFF\0ON\0REFR_ON\0REFR_OFF" );
  return *this;
}
