#include "Atm_button.hpp"

// Add option for button press callback (for reading i2c buttons etc)

Atm_button& Atm_button::begin( int attached_pin ) {
  // clang-format off
  const static state_t state_table[] PROGMEM = {
    /* Standard Mode: press/repeat */
    /*                  ON_ENTER  ON_LOOP       ON_EXIT  EVT_LMODE  EVT_TIMER  EVT_DELAY  EVT_REPEAT EVT_PRESS  EVT_RELEASE  EVT_COUNTER   EVT_AUTO  ELSE */
    /* IDLE     */            -1,      -1,           -1,     LIDLE,        -1,        -1,         -1,     WAIT,          -1,          -1,      AUTO,   -1,
    /* WAIT     */            -1,      -1,           -1,        -1,   PRESSED,        -1,         -1,       -1,        IDLE,          -1,        -1,    -1,
    /* PRESSED  */     ACT_PRESS,      -1,           -1,        -1,        -1,    REPEAT,         -1,       -1,     RELEASE,          -1,        -1,    -1,
    /* REPEAT   */     ACT_PRESS,      -1,           -1,        -1,        -1,        -1,     REPEAT,       -1,     RELEASE,          -1,        -1,    -1,
    /* RELEASE  */   ACT_RELEASE,      -1,           -1,        -1,        -1,        -1,         -1,       -1,          -1,          -1,        -1,  IDLE,
    /* Long Press Mode: press/long press */	
    /* LIDLE    */            -1,      -1,           -1,        -1,        -1,        -1,         -1,    LWAIT,          -1,          -1,        -1,    -1,
    /* LWAIT    */    ACT_LSTART,      -1,           -1,        -1,  LPRESSED,        -1,         -1,       -1,       LIDLE,          -1,        -1,    -1,
    /* LPRESSED */    ACT_LCOUNT,      -1,           -1,        -1,        -1,  LPRESSED,         -1,       -1,    LRELEASE,    WRELEASE,        -1,    -1,
    /* LRELEASE */  ACT_LRELEASE,      -1, ACT_WRELEASE,        -1,        -1,        -1,         -1,       -1,          -1,          -1,        -1, LIDLE,
    /* WRELEASE */  ACT_LRELEASE,      -1, ACT_WRELEASE,        -1,        -1,        -1,         -1,       -1,       LIDLE,          -1,        -1,    -1,
    /* AUTO     */      ACT_AUTO,      -1,           -1,        -1,        -1,        -1,         -1,       -1,          -1,          -1,        -1,  IDLE,
  };
  // clang-format on
  Machine::begin( state_table, ELSE );
  _pin = attached_pin;
  _counter_longpress.set( 0 );
  _timer_debounce.set( _DEBOUNCE );
  _timer_delay.set( ATM_TIMER_OFF );
  _timer_repeat.set( ATM_TIMER_OFF );
  _timer_auto.set( ATM_TIMER_OFF );
  pinMode( _pin, INPUT_PULLUP );
  return *this;
}

Atm_button& Atm_button::onPress( atm_button_cb_t callback, int idx /* = 0 */ ) {
  _onpress.set( (atm_cb_t)callback, idx );
  return *this;
}

Atm_button& Atm_button::onPress( Machine& machine, int event /* = 0 */ ) {
  _onpress.set( &machine, event );
  return *this;
}
Atm_button& Atm_button::onRelease( atm_button_cb_t callback, int idx /* = 0 */ ) {
  _onrelease.set( (atm_cb_t)callback, idx );
  return *this;
}

Atm_button& Atm_button::onRelease( Machine& machine, int event /* = 0 */ ) {
  _onrelease.set( &machine, event );
  return *this;
}

Atm_button& Atm_button::debounce( int delay ) {
  _timer_debounce.set( delay );
  return *this;
}

Atm_button& Atm_button::longPress( int max, int delay ) {
  _longpress_max = max;
  _counter_longpress.set( _longpress_max );
  _timer_delay.set( delay );
  return *this;
}

Atm_button& Atm_button::repeat( int delay /* = 500 */, int speed /* = 50 */ ) {
  _timer_delay.set( delay );
  _timer_repeat.set( speed );
  return *this;
}

Atm_button& Atm_button::autoPress( int delay, int press /* = 1 */ ) {
  _auto_press = press;
  _timer_auto.set( delay );
  return *this;
}

int Atm_button::event( int id ) {
  switch ( id ) {
    case EVT_LMODE:
      return _counter_longpress.value > 0;
    case EVT_TIMER:
      return _timer_debounce.expired( this );
    case EVT_DELAY:
      return _timer_delay.expired( this );
    case EVT_REPEAT:
      return _timer_repeat.expired( this );
    case EVT_AUTO:
      return _timer_auto.expired( this );
    case EVT_PRESS:
      return !digitalRead( _pin );
    case EVT_RELEASE:
      return digitalRead( _pin );
    case EVT_COUNTER:
      return _counter_longpress.expired();
  }
  return 0;
}

void Atm_button::action( int id ) {
  switch ( id ) {
    case ACT_PRESS:
    case ACT_AUTO:
      if ( !_onpress.push( true ) ) {
        ( *(atm_button_cb_t)_onpress.callback )( _onpress.callback_idx, 1 );
      }
      return;
    case ACT_RELEASE:
    case ACT_WRELEASE:
      if ( !_onrelease.push( true ) ) {
        ( *(atm_button_cb_t)_onrelease.callback )( _onrelease.callback_idx, 0 );
      }
      if ( _onpress.mode() == _onpress.MODE_CALLBACK ) {
        ( *(atm_button_cb_t)_onpress.callback )( _onpress.callback_idx, 0 );
      }
      return;
    case ACT_LSTART:
      _counter_longpress.set( _longpress_max );
      return;
    case ACT_LCOUNT:
      _counter_longpress.decrement();
      if ( _onpress.mode() == _onpress.MODE_CALLBACK ) {
        ( *(atm_button_cb_t)_onpress.callback )( _onpress.callback_idx, ( _longpress_max - _counter_longpress.value ) * -1 );
      }
      return;
    case ACT_LRELEASE:
      if ( _onpress.mode() == _onpress.MODE_CALLBACK ) {
        ( *(atm_button_cb_t)_onpress.callback )( _onpress.callback_idx, ( _longpress_max - _counter_longpress.value ) );
      }
      return;
  }
}

Atm_button& Atm_button::trace( Stream& stream ) {
  setTrace( &stream, atm_serial_debug::trace,
            "EVT_LMODE\0EVT_TIMER\0EVT_DELAY\0EVT_REPEAT\0EVT_PRESS\0EVT_RELEASE\0EVT_COUNTER\0EVT_"
            "AUTO\0ELSE\0IDLE\0WAIT\0PRESSED\0REPEAT\0RELEASE\0LIDLE\0LWAIT\0LPRESSED\0LRELEASE\0WRELEASE\0AUTO" );
  return *this;
}
