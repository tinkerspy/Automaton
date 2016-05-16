#include "Atm_led.hpp"

Atm_led& Atm_led::begin( int attached_pin, bool activeLow ) {
  // clang-format off
  static const STATE_TYPE state_table[] PROGMEM = {
    /*               ON_ENTER    ON_LOOP    ON_EXIT  EVT_ON_TIMER  EVT_OFF_TIMER  EVT_COUNTER  EVT_ON  EVT_OFF  EVT_BLINK  EVT_TOGGLE  EVT_TOGGLE_BLINK  ELSE */
    /* IDLE      */  ACT_INIT, ATM_SLEEP,        -1,           -1,            -1,          -1,     ON,      -1,     START,         ON,            START,   -1, // LED off
    /* ON        */    ACT_ON, ATM_SLEEP,        -1,           -1,            -1,          -1,     -1,     OFF,     START,        OFF,              OFF,   -1, // LED on
    /* START     */    ACT_ON,        -1,        -1,    BLINK_OFF,            -1,        DONE,     ON,     OFF,     START,        OFF,              OFF,   -1, // Start blinking
    /* BLINK_OFF */   ACT_OFF,        -1,        -1,           -1,         START,        DONE,     ON,    IDLE,     START,       IDLE,             OFF,   -1,
    /* DONE      */        -1,        -1, ACT_CHAIN,           -1,           OFF,          -1,     ON,    IDLE,     START,       IDLE,             OFF,   -1, // Wait after last blink
    /* OFF       */   ACT_OFF,        -1,        -1,           -1,            -1,          -1,     -1,      -1,        -1,         -1,               -1, IDLE, // All off -> IDLE
  };
  // clang-format on
  MACHINE::begin( state_table, ELSE );
  pin = attached_pin;
  _activeLow = activeLow;
  _level = 255;
  pinMode( pin, OUTPUT );
  digitalWrite( pin, _activeLow ? HIGH : LOW );
  on_timer.set( 500 );
  off_timer.set( 500 );
  repeat_count = ATM_COUNTER_OFF;
  counter.set( repeat_count );
  while ( state() != 0 ) cycle();
  return *this;
}

Atm_led& Atm_led::onFinish( Machine& machine, int event /* = 0 */ ) {
  _onfinish.set( &machine, event );
  return *this;
}

Atm_led& Atm_led::onFinish( TinyMachine& machine, int event /* = 0 */ ) {
  _onfinish.set( &machine, event );
  return *this;
}

Atm_led& Atm_led::onFinish( atm_cb_t callback, int idx /* = 0 */ ) {
  _onfinish.set( callback, idx );
  return *this;
}

#ifndef TINYMACHINE
Atm_led& Atm_led::onFinish( const char* label, int event /* = 0 */ ) {
  _onfinish.set( label, event );
  return *this;
}
#endif

Atm_led& Atm_led::blink( uint32_t duration, uint32_t pause_duration, uint16_t repeat_count /* = ATM_COUNTER_OFF */ ) {
  blink( duration );  // Time in which led is fully on
  pause( pause_duration );
  repeat( repeat_count );
  return *this;
}

Atm_led& Atm_led::blink( uint32_t duration ) {
  on_timer.set( duration );  // Time in which led is fully on
  return *this;
}

Atm_led& Atm_led::pause( uint32_t duration ) {
  off_timer.set( duration );  // Time in which led is fully off
  return *this;
}

Atm_led& Atm_led::fade( int fade ) {
  return *this;
}  // Dummy for method compatibility with Atm_fade

Atm_led& Atm_led::repeat( int repeat ) {
  repeat_count = repeat >= 0 ? repeat : ATM_COUNTER_OFF;
  counter.set( repeat_count );
  return *this;
}

Atm_led& Atm_led::brightness( uint8_t level ) {
  _level = level;
  if ( current == ON || current == START ) {
    analogWrite( pin, level );
  }
  return *this;
}

int Atm_led::event( int id ) {
  switch ( id ) {
    case EVT_ON_TIMER:
      return on_timer.expired( this );
    case EVT_OFF_TIMER:
      return off_timer.expired( this );
    case EVT_COUNTER:
      return counter.expired();
  }
  return 0;
}

void Atm_led::action( int id ) {
  switch ( id ) {
    case ACT_INIT:
      counter.set( repeat_count );
      return;
    case ACT_ON:
      if ( _activeLow ) {
        digitalWrite( pin, LOW );
      } else {
        if ( _level == 255 ) {
          digitalWrite( pin, HIGH );
        } else {
          analogWrite( pin, _level );
        }
      }
      return;
    case ACT_OFF:
      counter.decrement();
      if ( !_activeLow ) {
        digitalWrite( pin, LOW );
      } else {
        if ( _level == 255 ) {
          digitalWrite( pin, HIGH );
        } else {
          analogWrite( pin, _level );
        }
      }
      return;
    case ACT_CHAIN:
      _onfinish.push( FACTORY );
      return;
  }
}

Atm_led& Atm_led::trace( Stream& stream ) {
#ifndef TINYMACHINE
  setTrace( &stream, atm_serial_debug::trace,
            "EVT_ON_TIMER\0EVT_OFF_TIMER\0EVT_COUNTER\0EVT_ON\0EVT_OFF\0EVT_"
            "BLINK\0EVT_TOGGLE\0EVT_TOGGLE_BLINK\0ELSE\0"
            "IDLE\0ON\0START\0BLINK_OFF\0DONE\0OFF" );
#endif
  return *this;
}
