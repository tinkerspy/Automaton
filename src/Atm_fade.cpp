#include "Atm_fade.hpp"

Atm_fade& Atm_fade::begin( int attached_pin ) {
  // clang-format off
  const static state_t state_table[] PROGMEM = {
    /*               ON_ENTER    ON_LOOP       ON_EXIT  EVT_CNT_FADE EVT_TM_FADE   EVT_TM_ON  EVT_TM_OFF   EVT_CNT_RPT  EVT_ON EVT_OFF EVT_BLINK  EVT_TOGGLE  EVT_TOGGLE_BLINK    ELSE  */
    /* IDLE   */      ENT_OFF, ATM_SLEEP,           -1,           -1,         -1,         -1,         -1,           -1,     ON,   IDLE,    START,         ON,            START,     -1,  // LED off
    /* ON     */       ENT_ON, ATM_SLEEP,           -1,           -1,         -1,         -1,         -1,           -1,     ON,   IDLE,    START,       IDLE,             IDLE,     -1,  // LED on
    /* START  */      ENT_OFF,        -1,           -1,           -1,         -1,         -1,         -1,           -1,     ON,   IDLE,    START,       IDLE,             IDLE, STARTU,  // Start fading
    /* STARTU */    ENT_START,        -1,           -1,           -1,         -1,         -1,         UP,           -1,     ON,   IDLE,    START,       IDLE,             IDLE,     -1,  
    /* UP     */       ENT_UP,        -1,           -1,       STARTD,         UP,         -1,         -1,           -1,     ON,   IDLE,    START,       IDLE,             IDLE,     -1,
    /* STARTD */    ENT_START,        -1,           -1,           -1,         -1,       DOWN,         -1,           -1,     ON,   IDLE,    START,       IDLE,             IDLE,     -1,
    /* DOWN   */     ENT_DOWN,        -1,           -1,       REPEAT,       DOWN,         -1,         -1,           -1,     ON,   IDLE,    START,       IDLE,             IDLE,     -1,
    /* REPEAT */   ENT_REPEAT,        -1,           -1,           -1,         -1,         -1,         -1,         IDLE,     ON,   IDLE,    START,       IDLE,             IDLE, STARTU,
  };
  // clang-format on
  Machine::begin( state_table, ELSE );
  pin = attached_pin;
  pinMode( pin, OUTPUT );
  timer_fade.set( 0 );   // Number of ms per slope step (slope duration: rate * 32 ms)
  timer_on.set( 500 );   // Plateau between slopes (in which led is fully on)
  timer_off.set( 500 );  // Pause between slopes (in which led is fully off)
  counter_fade.set( SLOPE_SIZE );
  counter_repeat.set( ATM_COUNTER_OFF );
  repeat_count = ATM_COUNTER_OFF;
  return *this;
}

Atm_fade& Atm_fade::blink( uint32_t duration, uint32_t pause_duration, uint16_t repeat_count /* = ATM_COUNTER_OFF */ ) {
  blink( duration );  // Time in which led is fully on
  pause( pause_duration );
  repeat( repeat_count );
  return *this;
}

Atm_fade& Atm_fade::blink( uint32_t duration ) {
  timer_on.set( duration );  // Plateau between slopes (in which led is fully on)
  return *this;
}

Atm_fade& Atm_fade::pause( uint32_t duration ) {  // Pause between slopes (in which led is fully off)
  timer_off.set( duration ? duration : 1 );       // Make sure off_timer is never 0 (work around)
  return *this;
}

Atm_fade& Atm_fade::fade( int fade ) {
  timer_fade.set( fade >= 0 ? fade : ATM_TIMER_OFF );  // Number of ms per slope step (slope duration: rate * 32 ms)
  return *this;
}

Atm_fade& Atm_fade::repeat( uint16_t repeat ) {
  counter_repeat.set( repeat_count = repeat );
  return *this;
}

int Atm_fade::event( int id ) {
  switch ( id ) {
    case EVT_TM_FADE:
      return timer_fade.expired( this );
    case EVT_TM_ON:
      return timer_on.expired( this );
    case EVT_TM_OFF:
      return timer_off.expired( this );
    case EVT_CNT_FADE:
      return counter_fade.expired();
    case EVT_CNT_RPT:
      return counter_repeat.expired();
  }
  return 0;
}

void Atm_fade::action( int id ) {
  switch ( id ) {
    case ENT_ON:
      analogWrite( pin, 255 );
      return;
    case ENT_REPEAT:
      counter_repeat.decrement();
      return;
    case ENT_OFF:
      counter_repeat.set( repeat_count );
      analogWrite( pin, 0 );
      return;
    case ENT_START:
      counter_fade.set( SLOPE_SIZE );
      return;
    case ENT_UP:
      analogWrite( pin, slope[SLOPE_SIZE - counter_fade.value] );
      counter_fade.decrement();
      return;
    case ENT_DOWN:
      analogWrite( pin, slope[counter_fade.value - 1] );
      counter_fade.decrement();
      return;
  }
}

Atm_fade& Atm_fade::on( void ) {
  trigger( EVT_ON );
  return *this;
}

Atm_fade& Atm_fade::off( void ) {
  trigger( EVT_OFF );
  return *this;
}

Atm_fade& Atm_fade::toggle( void ) {
  trigger( EVT_TOGGLE );
  return *this;
}

Atm_fade& Atm_fade::toggleBlink( void ) {
  trigger( EVT_TOGGLE_BLINK );
  return *this;
}

Atm_fade& Atm_fade::start( void ) {
  trigger( EVT_BLINK );
  return *this;
}

Atm_fade& Atm_fade::trace( Stream& stream ) {
  setTrace( &stream, atm_serial_debug::trace,
            "FADE\0EVT_CNT_FADE\0EVT_TM_FADE\0EVT_TM_ON\0EVT_TM_OFF\0EVT_CNT_RPT\0EVT_ON\0EVT_OFF\0EVT_"
            "BLINK\0ELSE\0IDLE\0ON\0START\0STARTU\0UP\0STARTD\0DOWN\0REPEAT" );
  return *this;
}
