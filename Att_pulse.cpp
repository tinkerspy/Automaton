#include "Att_pulse.hpp"

#define FACTORY 0
#define STATE_TYPE tiny_state_t
#define MACHINE TinyMachine
#define TINYMACHINE

Att_pulse& Att_pulse::begin( int attached_pin, int minimum_duration /* = 20 */, bool activeLow /* = false */, bool pullUp /* = false */ ) {
  // clang-format off
  const static STATE_TYPE state_table[] PROGMEM = {
    /*              ON_ENTER    ON_LOOP      ON_EXIT  EVT_TIMER   EVT_HIGH  EVT_LOW   ELSE */
    /* IDLE    */         -1,        -1,          -1,        -1,      WAIT,      -1,    -1,
    /* WAIT    */         -1,        -1,          -1,     PULSE,        -1,    IDLE,    -1,
    /* PULSE   */  ACT_PULSE,        -1, ACT_RELEASE,        -1,        -1,    IDLE,    -1,
  };
  // clang-format on
  MACHINE::begin( state_table, ELSE );
  pin = attached_pin;
  _activeLow = activeLow;
  timer.set( minimum_duration );
  pinMode( pin, pullUp ? INPUT_PULLUP : INPUT );
  return *this;
}

Att_pulse& Att_pulse::onPulse( atm_cb_t callback, int idx /* = 0 */ ) {
  _onpulse.set( callback, idx );
  return *this;
}

Att_pulse& Att_pulse::onPulse( Machine& machine, int event /* = 0 */ ) {
  _onpulse.set( &machine, event );
  return *this;
}

Att_pulse& Att_pulse::onPulse( const char* label, int event /* = 0 */ ) {
  _onpulse.set( label, event );
  return *this;
}

Att_pulse& Att_pulse::onPulse( TinyMachine& machine, int event /* = 0 */ ) {
  _onpulse.set( &machine, event );
  return *this;
}

Att_pulse& Att_pulse::onRelease( atm_cb_t callback, int idx /* = 0 */ ) {
  _onrelease.set( callback, idx );
  return *this;
}

Att_pulse& Att_pulse::onRelease( Machine& machine, int event /* = 0 */ ) {
  _onrelease.set( &machine, event );
  return *this;
}

Att_pulse& Att_pulse::onRelease( const char* label, int event /* = 0 */ ) {
  _onrelease.set( label, event );
  return *this;
}

Att_pulse& Att_pulse::onRelease( TinyMachine& machine, int event /* = 0 */ ) {
  _onrelease.set( &machine, event );
  return *this;
}

int Att_pulse::event( int id ) {
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

void Att_pulse::action( int id ) {
  switch ( id ) {
    case ACT_PULSE:
      _onpulse.push( FACTORY );
      return;
    case ACT_RELEASE:
      _onrelease.push( FACTORY );
      return;
  }
}

#ifndef TINYMACHINE
Att_pulse& Att_pulse::trace( Stream& stream ) {
  setTrace( &stream, atm_serial_debug::trace, "EVT_TIMER\0EVT_HIGH\0EVT_LOW\0ELSE\0IDLE\0WAIT\0PULSE" );
  return *this;
}
#endif
