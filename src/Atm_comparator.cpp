#include "Atm_comparator.hpp"

Atm_comparator& Atm_comparator::begin( int attached_pin, int samplerate /* = 50 */ ) {
  // clang-format off
  const static state_t state_table[] PROGMEM = {
    /*              ON_ENTER    ON_LOOP  ON_EXIT  EVT_TRIGGER EVT_TIMER   ELSE */
    /* IDLE   */          -1,        -1,      -1,           -1,   SAMPLE,    -1,
    /* SAMPLE */  ACT_SAMPLE,        -1,      -1,         SEND,       -1,  IDLE,
    /* SEND   */    ACT_SEND,        -1,      -1,           -1,       -1,  IDLE,
  };
  // clang-format on
  Machine::begin( state_table, ELSE );
  pin = attached_pin;
  timer.set( samplerate );
  bitmap_sample = 0;
  bitmap_previous = 0;
  return *this;
}

int Atm_comparator::event( int id ) {
  switch ( id ) {
    case EVT_TRIGGER:
      if ( bitmap_diff ) {
        return 1;
      }
      return 0;
    case EVT_TIMER:
      return timer.expired( this );
  }
  return 0;
}

void Atm_comparator::action( int id ) {
  switch ( id ) {
    case ACT_SAMPLE:
      v_previous = v_sample;
      bitmap_previous = bitmap_sample;
      v_sample = sample();
      bitmap( v_sample );
      return;
    case ACT_SEND:
      if ( v_sample >= v_previous ) {
        for ( uint16_t i = 0; i < p_threshold_size; i++ ) {
          if ( ( bitmap_diff >> i ) & 1 ) {
            if ( !_onup.push( 0, 0, true ) ) {
              ( *(atm_comparator_cb_t)_onup.push_callback )( _onup.callback_idx, v_sample, 1, i, p_threshold[i] );
            }
          }
        }
      } else {
        for ( int i = p_threshold_size; i >= 0; i-- ) {
          if ( ( bitmap_diff >> i ) & 1 ) {
            if ( !_ondown.push( 0, 0, true ) ) {
              ( *(atm_comparator_cb_t)_ondown.push_callback )( _onup.callback_idx, v_sample, 0, i, p_threshold[i] );
            }
          }
        }
      }
      return;
  }
}

Atm_comparator& Atm_comparator::onChange( atm_comparator_cb_t callback, int idx /* = 0 */ ) {
  _onup.set( (atm_cb_push_t)callback, idx );
  _ondown.set( (atm_cb_push_t)callback, idx );
  return *this;
}

Atm_comparator& Atm_comparator::onChange( Machine& machine, int event /* = 0 */ ) {
  _onup.set( &machine, event );
  _ondown.set( &machine, event );
  return *this;
}

Atm_comparator& Atm_comparator::onChange( bool status, atm_comparator_cb_t callback, int idx /* = 0 */ ) {
  if ( status ) {
    _onup.set( (atm_cb_push_t)callback, idx );
  } else {
    _ondown.set( (atm_cb_push_t)callback, idx );
  }
  return *this;
}

Atm_comparator& Atm_comparator::onChange( bool status, Machine& machine, int event /* = 0 */ ) {
  if ( status ) {
    _onup.set( &machine, event );
  } else {
    _ondown.set( &machine, event );
  }
  return *this;
}

int Atm_comparator::read_sample() {
  return analogRead( pin );
}

int Atm_comparator::_avg() {
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

int Atm_comparator::sample() {
  return avg_buf_size > 0 ? _avg() : read_sample();
}

int Atm_comparator::state( void ) {
  return sample();
}

Atm_comparator& Atm_comparator::threshold( uint16_t* v, uint16_t size, bool catchUp /* = false */ ) {
  p_threshold = v;
  p_threshold_size = size;
  if ( !catchUp ) {
    v_sample = sample();
    bitmap( v_sample );
    v_previous = v_sample;
    bitmap_previous = bitmap_sample;
  }
  return *this;
}

Atm_comparator& Atm_comparator::average( uint16_t* v, uint16_t size ) {
  avg_buf = v;
  avg_buf_size = size;
  avg_buf_head = 0;
  avg_buf_total = 0;
  for ( uint16_t i = 0; i < avg_buf_size; i++ ) {
    avg_buf[i] = read_sample();
    avg_buf_total += avg_buf[i];
  }
  return *this;
}

Atm_comparator& Atm_comparator::bitmap( uint16_t v ) {
  bitmap_sample = 0;
  for ( uint8_t i = 0; i < p_threshold_size; i++ ) {
    if ( v >= p_threshold[i] ) bitmap_sample |= ( 1 << i );
  }
  bitmap_diff = bitmap_sample ^ bitmap_previous;
  return *this;
}

Atm_comparator& Atm_comparator::trace( Stream& stream ) {
#ifndef TINYMachine
  setTrace( &stream, atm_serial_debug::trace,
            "EVT_TRIGGER\0EVT_TIMER\0ELSE\0"
            "IDLE\0SAMPLE\0SEND" );

#endif
  return *this;
}
