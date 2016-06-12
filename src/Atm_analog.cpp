#include "Atm_analog.hpp"

Atm_analog& Atm_analog::begin( int attached_pin, int samplerate /* = 50 */ ) {
  // clang-format off
  const static state_t state_table[] PROGMEM = {
    /*              ON_ENTER    ON_LOOP  ON_EXIT  EVT_TRIGGER  EVT_TIMER   ELSE */
    /* IDLE   */          -1,        -1,      -1,          -1,   SAMPLE,    -1,
    /* SAMPLE */  ENT_SAMPLE,        -1,      -1,        SEND,       -1,  IDLE,
    /* SEND   */    ENT_SEND,        -1,      -1,          -1,       -1,  IDLE,
  };
  // clang-format on
  Machine::begin( state_table, ELSE );
  pin = attached_pin;
  timer.set( samplerate );
  return *this;
}

int Atm_analog::event( int id ) {
  switch ( id ) {
    case EVT_TIMER:
      return timer.expired( this );
    case EVT_TRIGGER:
      return v_previous != v_sample;
  }
  return 0;
}

void Atm_analog::action( int id ) {
  switch ( id ) {
    case ENT_SAMPLE:
      v_previous = v_sample;
      v_sample = sample();
      return;
    case ENT_SEND:
      v_sample = sample();
      onchange.push( v_sample, v_sample > v_previous );
      return;
  }
}

Atm_analog& Atm_analog::range( int toLow, int toHigh ) {
  this->toLow = toLow;
  this->toHigh = toHigh;
  return *this;
}

Atm_analog& Atm_analog::set( int value ) {  // Dummy method
  return *this;
}

Atm_analog& Atm_analog::onChange( Machine& machine, int event /* = 0 */ ) {
  this->onchange.set( &machine, event );
  return *this;
}

Atm_analog& Atm_analog::onChange( atm_cb_push_t callback, int idx /* = 0 */ ) {
  onchange.set( callback, idx );
  return *this;
}

int Atm_analog::read_sample() {
  return analogRead( pin );
}

int Atm_analog::avg() {
  uint16_t v = read_sample();
  avg_buf_total = avg_buf_total + v - avg_buf[avg_buf_head];
  avg_buf[avg_buf_head] = v;
  if ( avg_buf_head + 1 >= avg_buf_size ) {
    avg_buf_head = 0;
  } else {
    avg_buf_head++;
  }
  return avg_buf_total / avg_buf_size;
}

int Atm_analog::sample() {
  int v = avg_buf_size > 0 ? avg() : read_sample();
  if ( toHigh || toLow ) {
    return map( v, 0, 1023, toLow, toHigh );
  } else {
    return v;
  }
}

int Atm_analog::state( void ) {
  return sample();
}

Atm_analog& Atm_analog::average( uint16_t* v, uint16_t size ) {
  avg_buf = v;
  avg_buf_size = size / sizeof( uint16_t );
  avg_buf_head = 0;
  avg_buf_total = 0;
  for ( uint16_t i = 0; i < avg_buf_size; i++ ) {
    avg_buf[i] = read_sample();
    avg_buf_total += avg_buf[i];
  }
  return *this;
}

Atm_analog& Atm_analog::trace( Stream& stream ) {
  setTrace( &stream, atm_serial_debug::trace,
            "ANALOG\0EVT_TRIGGER\0EVT_TIMER\0ELSE\0"
            "IDLE\0SAMPLE\0SEND" );

  return *this;
}
