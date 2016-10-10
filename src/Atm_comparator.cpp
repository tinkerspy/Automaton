#include "Atm_comparator.hpp"

Atm_comparator& Atm_comparator::begin( int attached_pin, int samplerate /* = 50 */ ) {
  // clang-format off
  const static state_t state_table[] PROGMEM = {
    /*              ON_ENTER    ON_LOOP  ON_EXIT  EVT_TRIGGER EVT_TIMER   ELSE */
    /* IDLE   */          -1,        -1,      -1,           -1,   SAMPLE,    -1,
    /* SAMPLE */  ENT_SAMPLE,        -1,      -1,         SEND,       -1,  IDLE,
    /* SEND   */    ENT_SEND,        -1,      -1,           -1,       -1,  IDLE,
  };
  // clang-format on
  Machine::begin( state_table, ELSE );
  pin = attached_pin;
  timer.set( samplerate );
  bitmap_sample = 0;
  bitmap_previous = 0;
  skip_mode = 0;
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
    case ENT_SAMPLE:
      v_previous = v_sample;
      bitmap_previous = bitmap_sample;
      v_sample = sample();
      bitmap( v_sample );
      return;
    case ENT_SEND:
      int final_step = -1;
      if ( v_sample >= v_previous ) {
        for ( uint16_t i = 0; i < p_threshold_size; i++ ) {
          if ( ( bitmap_diff >> i ) & 1 ) {
            if ( skip_mode == 0 ) {
              onup.push( i, 1 );
            } else {
              final_step = i;
            }
          }
        }
      } else {
        for ( int i = p_threshold_size; i >= 0; i-- ) {
          if ( ( bitmap_diff >> i ) & 1 ) {
            if ( skip_mode == 0 ) {
              ondown.push( i, 0 );
            } else {
              final_step = i;
            }
          }
        }
      }
      if ( final_step > -1 ) {
        if ( v_sample >= v_previous ) {
          onup.push( final_step, 0 );
        } else {
          ondown.push( final_step, 0 );
        }
      }
      return;
  }
}

Atm_comparator& Atm_comparator::onChange( atm_cb_push_t callback, int idx /* = 0 */ ) {
  onup.set( callback, idx );
  ondown.set( callback, idx );
  return *this;
}

Atm_comparator& Atm_comparator::onChange( Machine& machine, int event /* = 0 */ ) {
  onup.set( &machine, event );
  ondown.set( &machine, event );
  return *this;
}

Atm_comparator& Atm_comparator::onChange( bool status, atm_cb_push_t callback, int idx /* = 0 */ ) {
  if ( status ) {
    onup.set( callback, idx );
  } else {
    ondown.set( callback, idx );
  }
  return *this;
}

Atm_comparator& Atm_comparator::onChange( bool status, Machine& machine, int event /* = 0 */ ) {
  if ( status ) {
    onup.set( &machine, event );
  } else {
    ondown.set( &machine, event );
  }
  return *this;
}

Atm_comparator& Atm_comparator::skip() {
  skip_mode = 1;
  return *this;
}

int Atm_comparator::read_sample() {
  return analogRead( pin );
}

int Atm_comparator::avg() {
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
  return avg_buf_size > 0 ? avg() : read_sample();
}

int Atm_comparator::state( void ) {
  return v_sample;
}

Atm_comparator& Atm_comparator::threshold( uint16_t* v, uint16_t size, bool catchUp /* = false */ ) {
  p_threshold = v;
  p_threshold_size = size / sizeof( uint16_t );
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
  avg_buf_size = size / sizeof( uint16_t );
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
  setTrace( &stream, atm_serial_debug::trace,
            "EVT_TRIGGER\0EVT_TIMER\0ELSE\0"
            "IDLE\0SAMPLE\0SEND" );
  return *this;
}
