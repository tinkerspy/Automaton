#include "Atm_button.hpp"

// Add option for button press callback (for reading i2c buttons etc)

Atm_button& Atm_button::begin( int attached_pin ) {
  // clang-format off
  const static state_t state_table[] PROGMEM = {
    /* Standard Mode: press/repeat */
    /*                  ON_ENTER  ON_LOOP       ON_EXIT  EVT_LMODE  EVT_TIMER  EVT_DELAY  EVT_REPEAT EVT_PRESS  EVT_RELEASE  EVT_COUNTER   EVT_AUTO  ELSE */
    /* IDLE     */            -1,      -1,           -1,     LIDLE,        -1,        -1,         -1,     WAIT,          -1,          -1,   AUTO_ST,   -1,
    /* WAIT     */            -1,      -1,           -1,        -1,   PRESSED,        -1,         -1,       -1,        IDLE,          -1,        -1,    -1,
    /* PRESSED  */     ENT_PRESS,      -1,           -1,        -1,        -1,    REPEAT,         -1,       -1,     RELEASE,          -1,        -1,    -1,
    /* REPEAT   */     ENT_PRESS,      -1,           -1,        -1,        -1,        -1,     REPEAT,       -1,     RELEASE,          -1,        -1,    -1,
    /* RELEASE  */   ENT_RELEASE,      -1,           -1,        -1,        -1,        -1,         -1,       -1,          -1,          -1,        -1,  IDLE,
    /* Long Press Mode: press/long press */	
    /* LIDLE    */            -1,      -1,           -1,        -1,        -1,        -1,         -1,    LWAIT,          -1,          -1,        -1,    -1,
    /* LWAIT    */    ENT_LSTART,      -1,           -1,        -1,  LPRESSED,        -1,         -1,       -1,       LIDLE,          -1,        -1,    -1,
    /* LPRESSED */    ENT_LCOUNT,      -1,           -1,        -1,        -1,  LPRESSED,         -1,       -1,    LRELEASE,    WRELEASE,        -1,    -1,
    /* LRELEASE */  ENT_LRELEASE,      -1, EXT_WRELEASE,        -1,        -1,        -1,         -1,       -1,          -1,          -1,        -1, LIDLE,
    /* WRELEASE */  ENT_LRELEASE,      -1, EXT_WRELEASE,        -1,        -1,        -1,         -1,       -1,       LIDLE,          -1,        -1,    -1,
    /* AUTO_ST   */      ENT_AUTO,      -1,           -1,        -1,        -1,        -1,         -1,       -1,          -1,          -1,        -1,  IDLE,
  };
  // clang-format on
  Machine::begin( state_table, ELSE );
  pin = attached_pin;
  counter_longpress.set( 0 );
  timer_debounce.set( DEBOUNCE );
  timer_delay.set( ATM_TIMER_OFF );
  timer_repeat.set( ATM_TIMER_OFF );
  timer_auto.set( ATM_TIMER_OFF );
  pinMode( pin, INPUT_PULLUP );
  return *this;
}

int Atm_button::event( int id ) {
  switch ( id ) {
    case EVT_LMODE:
      return counter_longpress.value > 0;
    case EVT_TIMER:
      return timer_debounce.expired( this );
    case EVT_DELAY:
      return timer_delay.expired( this );
    case EVT_REPEAT:
      return timer_repeat.expired( this );
    case EVT_AUTO:
      return timer_auto.expired( this );
    case EVT_PRESS:
      return !digitalRead( pin );
    case EVT_RELEASE:
      return digitalRead( pin );
    case EVT_COUNTER:
      return counter_longpress.expired();
  }
  return 0;
}

void Atm_button::action( int id ) {
  switch ( id ) {
    case ENT_PRESS:
      onpress.push( auto_press );
      return;
    case ENT_AUTO:
      onpress.push( 1 );
      return;
    case ENT_RELEASE:
    case EXT_WRELEASE:
      onrelease.push( 0 );
      return;
    case ENT_LSTART:
      counter_longpress.set( longpress_max );
      return;
    case ENT_LCOUNT:
      counter_longpress.decrement();
      if ( onpress.mode() == onpress.MODE_PUSHCB ) {
        onpress.push( ( longpress_max - counter_longpress.value ) * -1 );
      }
      return;
    case ENT_LRELEASE:
      if ( onpress.mode() == onpress.MODE_PUSHCB ) {
        onpress.push( longpress_max - counter_longpress.value );
      }
      return;
  }
}

Atm_button& Atm_button::onPress( atm_cb_push_t callback, int idx /* = 0 */ ) {
  onpress.set( callback, idx );
  return *this;
}

Atm_button& Atm_button::onPress( Machine& machine, int event /* = 0 */ ) {
  onpress.set( &machine, event );
  return *this;
}
Atm_button& Atm_button::onRelease( atm_cb_push_t callback, int idx /* = 0 */ ) {
  onrelease.set( callback, idx );
  return *this;
}

Atm_button& Atm_button::onRelease( Machine& machine, int event /* = 0 */ ) {
  onrelease.set( &machine, event );
  return *this;
}

Atm_button& Atm_button::debounce( int delay ) {
  timer_debounce.set( delay );
  return *this;
}

Atm_button& Atm_button::longPress( int max, int delay ) {
  longpress_max = max;
  counter_longpress.set( longpress_max );
  timer_delay.set( delay );
  return *this;
}

Atm_button& Atm_button::repeat( int delay /* = 500 */, int speed /* = 50 */ ) {
  timer_delay.set( delay );
  timer_repeat.set( speed );
  return *this;
}

Atm_button& Atm_button::autoPress( int delay, int press /* = 1 */ ) {
  auto_press = press;
  timer_auto.set( delay );
  return *this;
}

Atm_button& Atm_button::trace( Stream& stream ) {
  setTrace( &stream, atm_serial_debug::trace,
            "BUTTON\0EVT_LMODE\0EVT_TIMER\0EVT_DELAY\0EVT_REPEAT\0EVT_PRESS\0EVT_RELEASE\0EVT_COUNTER\0EVT_"
            "AUTO_ST\0ELSE\0IDLE\0WAIT\0PRESSED\0REPEAT\0RELEASE\0LIDLE\0LWAIT\0LPRESSED\0LRELEASE\0WRELEASE\0AUTO" );
  return *this;
}
