#include "Atm_led.hpp"

Atm_led& Atm_led::begin( int attached_pin, bool activeLow, byte* blinkPattern, int blinkPatternSize ) {
  // clang-format off
  static const state_t state_table[] PROGMEM = {
    /*               ON_ENTER    ON_LOOP    ON_EXIT  EVT_ON_TIMER  EVT_OFF_TIMER EVT_WT_TIMER EVT_COUNTER  EVT_ON  EVT_OFF  EVT_BLINK  EVT_TOGGLE  EVT_TOGGLE_BLINK   ELSE */
    /* IDLE      */  ENT_INIT, ATM_SLEEP,        -1,           -1,            -1,          -1,         -1,  WT_ON,      -1,  WT_START,         ON,         WT_START,    -1, // LED off
    /* ON        */    ENT_ON, ATM_SLEEP,        -1,           -1,            -1,          -1,         -1,     -1,     OFF,  WT_START,        OFF,              OFF,    -1, // LED on
    /* START     */    ENT_ON,        -1,        -1,    BLINK_OFF,            -1,          -1,         -1,  WT_ON,     OFF,        -1,        OFF,              OFF,    -1, // Start blinking
    /* BLINK_OFF */   ENT_OFF,        -1,        -1,           -1,          LOOP,          -1,         -1,  WT_ON,     OFF,        -1,        OFF,              OFF,    -1,
    /* LOOP      */        -1,        -1,        -1,           -1,            -1,          -1,       DONE,  WT_ON,     OFF,        -1,        OFF,              OFF, START,    
    /* DONE      */        -1,        -1, EXT_CHAIN,           -1,           OFF,          -1,         -1,  WT_ON,     OFF,  WT_START,        OFF,              OFF,    -1, // Wait after last blink
    /* OFF       */   ENT_OFF,        -1,        -1,           -1,            -1,          -1,         -1,  WT_ON,     OFF,  WT_START,         -1,               -1,  IDLE, // All off -> IDLE
    /* WT_ON     */        -1,        -1,        -1,           -1,            -1,          ON,         -1,  WT_ON,     OFF,  WT_START,         -1,               -1,    -1, // LEAD for ON
    /* WT_START  */        -1,        -1,        -1,           -1,            -1,       START,         -1,  WT_ON,     OFF,  WT_START,         -1,               -1,    -1, // LEAD for BLINK
  }; 
  // clang-format on
  Machine::begin( state_table, ELSE );
  pin = attached_pin;
  this->activeLow = activeLow;
  level = 255;
  toLow = 0;
  toHigh = 255;
  wrap = false;
  pinMode( pin, OUTPUT );
  digitalWrite( pin, activeLow ? HIGH : LOW );
  pattern = blinkPattern;
  patternSize = blinkPatternSize;
  reset();
  while ( state() != 0 ) cycle();
  return *this;
}

void Atm_led::reset() {
  off_timer.set( offDuration );
  if( pattern == NULL ) {
    on_timer.set( onDuration );
    repeat_count = ATM_COUNTER_OFF;
  } else {
    repeat_count = patternSize;
    on_timer.set( onDuration * pattern[0] );
  }
  counter.set( repeat_count );
}

int Atm_led::event( int id ) {
  switch ( id ) {
    case EVT_ON_TIMER:
      return on_timer.expired( this );
    case EVT_OFF_TIMER:
      return off_timer.expired( this );
    case EVT_WT_TIMER:
      return lead_timer.expired( this );
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
      if ( on_timer.value > 0 ) { // Never turn if on_timer is zero (duty cycle 0 must be dark)
        if ( activeLow ) {
          digitalWrite( pin, LOW );
        } else {
          if ( level == toHigh ) {
            digitalWrite( pin, HIGH );
          } else {
            analogWrite( pin, mapLevel( level ) );
          }
        }
      }
      return;
    case ENT_OFF:
      counter.decrement();
      if ( !activeLow ) {
        digitalWrite( pin, LOW );
      } else {
        if ( level == toHigh ) {
          digitalWrite( pin, HIGH );
        } else {
          analogWrite( pin, mapLevel( level ) );
        }
      }
      if( state() == OFF ) {
        // End of sequence, reset the counter in case it gets called again (eg, by an OnFinish connector)
        counter.set( repeat_count );
      }
      if(pattern != NULL ) {
        // Change the timer for the next On cycle.
        int i = patternSize - counter.value;
        on_timer.set(onDuration * pattern[i]);
      }
      return;
    case EXT_CHAIN:
      reset();
      onfinish.push( 0 );
      return;
  }
}

int Atm_led::mapLevel( int level ) {
  if ( levelMapSize ) {
    return levelMap[level];
  } else {
    return map( level, toLow, toHigh, 0, 255 );
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
  if( patternSize == 0)
    repeat( repeat_count );
  return *this;
}

Atm_led& Atm_led::blink( uint32_t duration ) {
  onDuration = duration;
  on_timer.set( duration );  // Time in which led is fully on
  return *this;
}

Atm_led& Atm_led::blink( void ) {
  trigger( EVT_BLINK );
  return *this;
}

Atm_led& Atm_led::range( int toLow, int toHigh, bool wrap /* = false */ ) {
  this->toLow = toLow; 
  this->toHigh = toHigh; 
  this->wrap = wrap;
  level = toHigh;
  return *this;
}

Atm_led& Atm_led::levels( unsigned char* map, int mapsize, bool wrap /* = false */ ) {
  this->levelMap = map;
  levelMapSize = mapsize;
  range( 0, mapsize - 1, wrap );
  return *this;
}

Atm_led& Atm_led::pause( uint32_t duration ) {  // Time in which led is fully off
  offDuration = duration;
  off_timer.set( duration ? duration : 1 );     // Make sure off_timer is never 0 (work around)
  return *this;
}

Atm_led& Atm_led::setpattern( byte* blinkPattern, uint32_t blinkPatternSize ) {
  pattern = blinkPattern;
  this->patternSize = blinkPatternSize;
  return *this;
}

Atm_led& Atm_led::fade( int fade ) {
  return *this;
}  // Dummy for method compatibility with Atm_fade

Atm_led& Atm_led::lead( uint32_t ms ) {
  lead_timer.set( ms );
  return *this;
} 

Atm_led& Atm_led::repeat( uint16_t repeat ) {
  counter.set( repeat_count = repeat );
  return *this;
}

int Atm_led::brightness( int level /* = -1 */ ) {
  if ( level > -1 ) {
    this->level = level;
    if ( current == ON || current == START ) {
      analogWrite( pin, mapLevel( level ) );
    }
  }
  return this->level;
}

int Atm_led::brighten( int v ) {
  if ( abs( v ) == 1 ) {
    int br = (int)this->level + v;
    if ( br > toHigh ) 
      br = wrap ? toLow : toHigh;
    if ( br < toLow ) 
      br = wrap ? toHigh : toLow;
    brightness( br );
  }
  return this->level;
}

Atm_led& Atm_led::trigger( int event ) {
  if ( event > ELSE ) {
    brighten( event == EVT_BRUP ? 1 : -1 );
  } else {
    Machine::trigger( event );
  }
  return *this;
}

Atm_led& Atm_led::trace( Stream& stream ) {
  setTrace( &stream, atm_serial_debug::trace,
            "LED\0EVT_ON_TIMER\0EVT_OFF_TIMER\0EVT_WT_TIMER\0EVT_COUNTER\0EVT_ON\0EVT_OFF\0EVT_"
            "BLINK\0EVT_TOGGLE\0EVT_TOGGLE_BLINK\0ELSE\0"
            "IDLE\0ON\0START\0BLINK_OFF\0LOOP\0DONE\0OFF\0WT_ON\0WT_START" );
  return *this;
}
