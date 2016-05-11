#include "Att_comparator.hpp"

Att_comparator& Att_comparator::begin( int attached_pin, int samplerate /* = 50 */ ) {
  // clang-format off
  const static STATE_TYPE state_table[] PROGMEM = {
    /*              ON_ENTER    ON_LOOP  ON_EXIT  EVT_TRIGGER EVT_TIMER   ELSE */
    /* IDLE   */          -1,        -1,      -1,           -1,   SAMPLE,    -1,
    /* SAMPLE */  ACT_SAMPLE,        -1,      -1,         SEND,       -1,  IDLE,
    /* SEND   */    ACT_SEND,        -1,      -1,           -1,       -1,  IDLE,
  };
  // clang-format on
  MACHINE::begin( state_table, ELSE );
  pin = attached_pin;
  timer.set( samplerate );
  bitmap_sample = 0;
  bitmap_previous = 0;
  return *this;
}

Att_comparator& Att_comparator::onUp( atm_comparator_cb_t callback, int16_t idx /* = 0 */ ) {
  _onup.set( (atm_cb_t)callback, idx );
  return *this;
}

Att_comparator& Att_comparator::onUp( Machine& machine, int event /* = 0 */ ) {
  _onup.set( &machine, event );
  return *this;
}

Att_comparator& Att_comparator::onUp( TinyMachine& machine, int event /* = 0 */ ) {
  _onup.set( &machine, event );
  return *this;
}

#ifndef TINYMACHINE
Att_comparator& Att_comparator::onUp( const char* label, int event /* = 0 */ ) {
  _onup.set( label, event );
  return *this;
}
#endif

Att_comparator& Att_comparator::onDown( atm_comparator_cb_t callback, int16_t idx /* = 0 */ ) {
  _ondown.set( (atm_cb_t)callback, idx );
  return *this;
}

Att_comparator& Att_comparator::onDown( Machine& machine, int event /* = 0 */ ) {
  _ondown.set( &machine, event );
  return *this;
}

Att_comparator& Att_comparator::onDown( TinyMachine& machine, int event /* = 0 */ ) {
  _ondown.set( &machine, event );
  return *this;
}

#ifndef TINYMACHINE
Att_comparator& Att_comparator::onDown( const char* label, int event /* = 0 */ ) {
  _ondown.set( label, event );
  return *this;
}
#endif

int Att_comparator::read_sample() {
  return analogRead( pin );
}

int Att_comparator::_avg() {
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

int Att_comparator::sample() {
  return avg_buf_size > 0 ? _avg() : read_sample();
}

Att_comparator& Att_comparator::threshold( uint16_t* v, uint16_t size, bool catchUp /* = false */ ) {
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

Att_comparator& Att_comparator::average( uint16_t* v, uint16_t size ) {
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

Att_comparator& Att_comparator::bitmap( uint16_t v ) {
  bitmap_sample = 0;
  for ( uint8_t i = 0; i < p_threshold_size; i++ ) {
    if ( v >= p_threshold[i] ) bitmap_sample |= ( 1 << i );
  }
  bitmap_diff = bitmap_sample ^ bitmap_previous;
  return *this;
}

int Att_comparator::event( int id ) {
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

void Att_comparator::action( int id ) {
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
            if ( !_onup.push( FACTORY, true ) ) {
              ( *(atm_comparator_cb_t)_onup.callback )( _onup.callback_idx, v_sample, 1, i, p_threshold[i] );
            }
          }
        }
      } else {
        for ( int i = p_threshold_size; i >= 0; i-- ) {
          if ( ( bitmap_diff >> i ) & 1 ) {
            if ( !_ondown.push( FACTORY, true ) ) {
              ( *(atm_comparator_cb_t)_ondown.callback )( _onup.callback_idx, v_sample, 0, i, p_threshold[i] );
            }
          }
        }
      }
      return;
  }
}

#ifndef TINYMACHINE
Att_comparator& Att_comparator::trace( Stream& stream ) {
  setTrace( &stream, atm_serial_debug::trace,
            "EVT_TRIGGER\0EVT_TIMER\0ELSE\0"
            "IDLE\0SAMPLE\0SEND" );

  return *this;
}
#endif
