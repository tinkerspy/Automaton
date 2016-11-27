#include "Atm_timer.hpp"

// Timer class that handles intervals from 1 millisecond up to 136 years
// Combined with repeat( 65534 ) that makes a maximum of 8.9 million years

#define DIVIDER 86400  // Number of seconds in a 24h day

Atm_timer& Atm_timer::begin( uint32_t ms /* = ATM_TIMER_OFF */, uint16_t repeats /* = 1 */ ) {
  // clang-format off
  const static state_t state_table[] PROGMEM = {
    /*               ON_ENTER    ON_LOOP    ON_EXIT  EVT_DAYCNT  EVT_DAYTIMER  EVT_MSTIMER  EVT_REPCNT  EVT_STOP  EVT_START  EVT_TOGGLE   ELSE */
    /* IDLE    */          -1, ATM_SLEEP,        -1,         -1,           -1,          -1,         -1,       -1,     START,      START,    -1,
    /* START   */   ENT_START,        -1,        -1,         -1,           -1,          -1,         -1,       -1,     WAITD,         -1, WAITD,  
    /* WAITD   */          -1,        -1, EXT_WAITD,     WAITMS,        WAITD,          -1,         -1,     IDLE,     START,       IDLE,    -1,
    /* WAITMS  */          -1,        -1,        -1,         -1,           -1,     TRIGGER,         -1,     IDLE,     START,       IDLE,    -1,
    /* TRIGGER */ ENT_TRIGGER,        -1,        -1,         -1,           -1,          -1,     FINISH,     IDLE,     START,       IDLE, START,
    /* FINISH  */  ENT_FINISH,        -1,        -1,         -1,           -1,          -1,         -1,       -1,        -1,         -1,  IDLE,
  };
  // clang-format on
  Machine::begin( state_table, ELSE );
  daytimer.set( (uint32_t)DIVIDER * 1000 );  // Always set to one day
  mstimer.set( ms );
  daycounter.set( days = 0 );
  repeat( repeat_cnt = repeats );
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
    case ENT_START:
      daycounter.set( days );
      return;
    case EXT_WAITD:
      daycounter.decrement();
      return;
    case ENT_TRIGGER:
      abscounter++;
      repcounter.decrement();
      ontimer.push( repcounter.value, abscounter );
      return;
    case ENT_FINISH:
      onfinish.push( 0 );
      repcounter.set( repeat_cnt );
      abscounter = 0;
      return;
  }
}

Atm_timer& Atm_timer::onTimer( atm_cb_push_t callback, int idx /* = 0 */ ) {
  ontimer.set( callback, idx );
  return *this;
}

Atm_timer& Atm_timer::onTimer( Machine& machine, int event /* = 0 */ ) {
  ontimer.set( &machine, event );
  return *this;
}

Atm_timer& Atm_timer::onFinish( atm_cb_push_t callback, int idx /* = 0 */ ) {
  onfinish.set( callback, idx );
  return *this;
}

Atm_timer& Atm_timer::onFinish( Machine& machine, int event /* = 0 */ ) {
  onfinish.set( &machine, event );
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

Atm_timer& Atm_timer::repeat( uint16_t v /* = ATM_COUNTER_OFF */ ) {
  repeat_cnt = v;
  repcounter.set( v );
  return *this;
}

uint32_t Atm_timer::left() {
  uint32_t r = daycounter.value * DIVIDER;
  r += mstimer.value - ( millis() - state_millis );
  return current == WAITD || current == WAITMS ? r : 0;
}

Atm_timer& Atm_timer::start( void ) {
  trigger( EVT_START );
  return *this;
}

Atm_timer& Atm_timer::stop( void ) {
  trigger( EVT_STOP );
  return *this;
}

Atm_timer& Atm_timer::toggle( void ) {
  trigger( EVT_TOGGLE );
  return *this;
}

Atm_timer& Atm_timer::trace( Stream& stream ) {
  setTrace( &stream, atm_serial_debug::trace,
            "TIMER\0EVT_DAYCNT\0EVT_DAYTIMER\0EVT_MSTIMER\0EVT_REPCNT\0EVT_OFF\0EVT_START\0EVT_TOGGLE\0"
            "ELSE\0IDLE\0START\0WAITD\0WAITMS\0TRIGGER\0FINISH" );
  return *this;
}
