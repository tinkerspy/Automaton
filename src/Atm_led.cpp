#include "Atm_led.hpp"

Atm_led& Atm_led::begin( int attached_pin, bool activeLow ) {
  // clang-format off
  static const state_t state_table[] PROGMEM = {
    /*               ON_ENTER    ON_LOOP    ON_EXIT  EVT_ON_TIMER  EVT_OFF_TIMER  EVT_COUNTER  EVT_ON  EVT_OFF  EVT_BLINK  EVT_TOGGLE  EVT_TOGGLE_BLINK  ELSE */
    /* IDLE      */  ENT_INIT, ATM_SLEEP,        -1,           -1,            -1,          -1,     ON,      -1,     START,         ON,            START,    -1, // LED off
    /* ON        */    ENT_ON, ATM_SLEEP,        -1,           -1,            -1,          -1,     -1,     OFF,     START,        OFF,              OFF,    -1, // LED on
    /* START     */    ENT_ON,        -1,        -1,    BLINK_OFF,            -1,          -1,     ON,     OFF,        -1,        OFF,              OFF,    -1, // Start blinking
    /* BLINK_OFF */   ENT_OFF,        -1,        -1,           -1,          LOOP,          -1,     ON,     OFF,        -1,        OFF,              OFF,    -1,
    /* LOOP      */        -1,        -1,        -1,           -1,            -1,        DONE,     ON,     OFF,        -1,        OFF,              OFF, START,    
    /* DONE      */        -1,        -1, EXT_CHAIN,           -1,           OFF,          -1,     ON,     OFF,     START,        OFF,              OFF,    -1, // Wait after last blink
    /* OFF       */   ENT_OFF,        -1,        -1,           -1,            -1,          -1,     ON,     OFF,     START,         -1,               -1,  IDLE, // All off -> IDLE
  };
  // clang-format on
  Machine::begin( state_table, ELSE );
  pin = attached_pin;
  this->activeLow = activeLow;
  level = 255;
  pinMode( pin, OUTPUT );
  digitalWrite( pin, activeLow ? HIGH : LOW );
  on_timer.set( 500 );
  off_timer.set( 500 );
  repeat_count = ATM_COUNTER_OFF;
  counter.set( repeat_count );
  while ( state() != 0 ) cycle();
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
    case ENT_INIT:
      counter.set( repeat_count );
      return;
    case ENT_ON:
      if ( activeLow ) {
        digitalWrite( pin, LOW );
      } else {
        if ( level == 255 ) {
          digitalWrite( pin, HIGH );
        } else {
          analogWrite( pin, level );
        }
      }
      return;
    case ENT_OFF:
      counter.decrement();
      if ( !activeLow ) {
        digitalWrite( pin, LOW );
      } else {
        if ( level == 255 ) {
          digitalWrite( pin, HIGH );
        } else {
          analogWrite( pin, level );
        }
      }
      return;
    case EXT_CHAIN:
      onfinish.push( 0 );
      return;
  }
}

Atm_led& Atm_led::on( void ) {
  trigger( EVT_ON );
  return *this;
}

Atm_led& Atm_led::off( void ) {
  trigger( EVT_OFF );
  return *this;
}

Atm_led& Atm_led::toggle( void ) {
  trigger( EVT_TOGGLE );
  return *this;
}

Atm_led& Atm_led::toggleBlink( void ) {
  trigger( EVT_TOGGLE_BLINK );
  return *this;
}

Atm_led& Atm_led::start( void ) {
  trigger( EVT_BLINK );
  return *this;
}

Atm_led& Atm_led::onFinish( Machine& machine, int event /* = 0 */ ) {
  onfinish.set( &machine, event );
  return *this;
}

Atm_led& Atm_led::onFinish( atm_cb_push_t callback, int idx /* = 0 */ ) {
  onfinish.set( callback, idx );
  return *this;
}

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

Atm_led& Atm_led::pause( uint32_t duration ) {  // Time in which led is fully off
  off_timer.set( duration ? duration : 1 );     // Make sure off_timer is never 0 (work around)
  return *this;
}

Atm_led& Atm_led::fade( int fade ) {
  return *this;
}  // Dummy for method compatibility with Atm_fade

Atm_led& Atm_led::repeat( uint16_t repeat ) {
  counter.set( repeat_count = repeat );
  return *this;
}

Atm_led& Atm_led::brightness( uint8_t level ) {
  this->level = level;
  if ( current == ON || current == START ) {
    analogWrite( pin, level );
  }
  return *this;
}

Atm_led& Atm_led::trace( Stream& stream ) {
  setTrace( &stream, atm_serial_debug::trace,
            "LED\0EVT_ON_TIMER\0EVT_OFF_TIMER\0EVT_COUNTER\0EVT_ON\0EVT_OFF\0EVT_"
            "BLINK\0EVT_TOGGLE\0EVT_TOGGLE_BLINK\0ELSE\0"
            "IDLE\0ON\0START\0BLINK_OFF\0LOOP\0DONE\0OFF" );
  return *this;
}
