#include "Atm_timer.hpp"

// Timer class that handles intervals from 1 millisecond up to 136 years
// Combined with repeat( 65534 ) that makes a maximum of 8.9 million years

#define DIVIDER 86400  // Number of seconds in a 24h day

Atm_timer& Atm_timer::begin( uint32_t ms /* = ATM_TIMER_OFF */ ) {
  // clang-format off
  const static STATE_TYPE state_table[] PROGMEM = {
    /*             ON_ENTER    ON_LOOP    ON_EXIT  EVT_DAYCNT  EVT_DAYTIMER  EVT_MSTIMER  EVT_REPCNT  EVT_OFF  EVT_ON   ELSE */
    /* IDLE    */        -1, ATM_SLEEP,        -1,         -1,           -1,          -1,         -1,      -1,  START,    -1,
    /* START   */ ACT_START,        -1,        -1,         -1,           -1,          -1,         -1,      -1,  WAITD, WAITD,  
    /* WAITD   */        -1,        -1, ACT_WAITD,     WAITMS,        WAITD,          -1,         -1,    IDLE,  START,    -1,
    /* WAITMS  */        -1,        -1,        -1,         -1,           -1,     TRIGGER,         -1,    IDLE,  START,    -1,
    /* TRIGGER */  ACT_TRIG,        -1,        -1,         -1,           -1,          -1,       IDLE,    IDLE,  START, START,
  };
  // clang-format on
  MACHINE::begin( state_table, ELSE );
  daytimer.set( (uint32_t)DIVIDER * 1000 );  // Always set to one day
  mstimer.set( ms );
  daycounter.set( days = 0 );
  repeat( repeat_cnt = 1 );
  return *this;
}

Atm_timer& Atm_timer::onTimer( atm_cb_t callback, int idx /* = 0 */ ) {
  _ontimer.set( callback, idx );
  return *this;
}

Atm_timer& Atm_timer::onTimer( Machine& machine, int event /* = 0 */ ) {
  _ontimer.set( &machine, event );
  return *this;
}

Atm_timer& Atm_timer::onTimer( const char* label, int event /* = 0 */ ) {
  _ontimer.set( label, event );
  return *this;
}

Atm_timer& Atm_timer::onTimer( TinyMachine& machine, int event /* = 0 */ ) {
  _ontimer.set( &machine, event );
  return *this;
}

Atm_timer& Atm_timer::interval_seconds( uint32_t v ) {
  days = v / ( (uint32_t)DIVIDER );
  daycounter.set( days );                                      // Determine how many days -> days => Set day counter
  mstimer.set( ( v - ( (uint32_t)days * DIVIDER ) ) * 1000 );  // And how many milliseconds left
  return *this;
}

Atm_timer& Atm_timer::interval_millis( uint32_t v ) {
  days = 0;
  daycounter.set( days );
  mstimer.set( v );
  return *this;
}

Atm_timer& Atm_timer::interval( uint32_t v ) {
  return interval_millis( v );
}

Atm_timer& Atm_timer::repeat( uint16_t v ) {
  repeat_cnt = v;
  repcounter.set( v );
  return *this;
}

int Atm_timer::event( int id ) {
  switch ( id ) {
    case EVT_REPCNT:
      return repcounter.expired();
    case EVT_DAYCNT:
      return daycounter.expired();
    case EVT_MSTIMER:
      return mstimer.expired( this );
    case EVT_DAYTIMER:
      return daytimer.expired( this );
  }
  return 0;
}

void Atm_timer::action( int id ) {
  switch ( id ) {
    case ACT_START:
      daycounter.set( days );
      return;
    case ACT_WAITD:
      daycounter.decrement();
      return;
    case ACT_TRIG:
      repcounter.decrement();
      _ontimer.push( FACTORY );
      return;
  }
}

Atm_timer& Atm_timer::trace( Stream& stream ) {
#ifndef TINYMACHINE
  setTrace( &stream, atm_serial_debug::trace, "EVT_DAYCNT\0EVT_DAYTIMER\0EVT_MSTIMER\0EVT_REPCNT\0EVT_OFF\0EVT_ON\0ELSE\0IDLE\0START\0WAITD\0WAITMS\0TRIGGER" );
#endif
  return *this;
}
