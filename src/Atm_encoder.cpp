#include "Atm_encoder.hpp"
#include <limits.h>

// Loosely based on https://www.circuitsathome.com/mcu/reading-rotary-encoder-on-arduino (Oleg Mazurov)

const char Atm_encoder::enc_states[16] = {0, (char)-1, 1, 0, 1, 0, 0, (char)-1, (char)-1, 0, 0, 1, 0, 1, (char)-1, 0};

Atm_encoder& Atm_encoder::begin( int pin1, int pin2, int divider /* = 1 */ ) {
  // clang-format off
  const static state_t state_table[] PROGMEM = {
    /*          ON_ENTER     ON_LOOP  ON_EXIT  EVT_UP  EVT_DOWN  ELSE */
    /* IDLE  */       -1,    LP_IDLE,      -1,     UP,     DOWN,   -1,
    /* UP    */   ENT_UP,         -1,      -1,     -1,       -1, IDLE,
    /* DOWN  */ ENT_DOWN,         -1,      -1,     -1,       -1, IDLE,
  };
  // clang-format on
  Machine::begin( state_table, ELSE );
  this->pin1 = pin1;
  this->pin2 = pin2;
  this->divider = divider;
  pinMode( pin1, INPUT );
  pinMode( pin2, INPUT );
  digitalWrite( pin1, HIGH );
  digitalWrite( pin2, HIGH );
  min = INT_MIN;
  max = INT_MAX;
  value = 0;
  return *this;
}

int Atm_encoder::event( int id ) {
  switch ( id ) {
    case EVT_UP:
      return enc_direction == +1 && ( enc_counter % divider == 0 );
    case EVT_DOWN:
      return enc_direction == -1 && ( enc_counter % divider == 0 );
  }
  return 0;
}

void Atm_encoder::action( int id ) {
  switch ( id ) {
    case LP_IDLE:
      enc_bits = ( ( enc_bits << 2 ) | ( digitalRead( pin1 ) << 1 ) | ( digitalRead( pin2 ) ) ) & 0x0f;
      enc_direction = enc_states[enc_bits];
      if ( enc_direction != 0 ) {
        enc_counter = enc_counter + enc_direction;
        if ( ( enc_counter != 0 ) && ( enc_counter % divider == 0 ) ) {
          if ( !count( enc_direction ) ) {
            enc_direction = 0;
          }
        }
      }
      return;
    case ENT_UP:
      onup.push( state(), 1 );
      return;
    case ENT_DOWN:
      ondown.push( state(), 0 );
      return;
  }
}

Atm_encoder& Atm_encoder::range( int min, int max, bool wrap /* = false */ ) {
  if ( min > max ) {
    range_invert = true;
    this->min = max;
    this->max = min;
  } else {
    range_invert = false;
    this->min = min;
    this->max = max;
  }
  this->wrap = wrap;
  if ( value < min || value > max ) {
    value = min;
  }
  return *this;
}

Atm_encoder& Atm_encoder::set( int value ) {
  this->value = range_invert ? map( value, min, max, max, min ) : value;
  return *this;
}

Atm_encoder& Atm_encoder::onChange( Machine& machine, int event /* = 0 */ ) {
  onup.set( &machine, event );
  ondown.set( &machine, event );
  return *this;
}

Atm_encoder& Atm_encoder::onChange( atm_cb_push_t callback, int idx /* = 0 */ ) {
  onup.set( callback, idx );
  ondown.set( callback, idx );
  return *this;
}

Atm_encoder& Atm_encoder::onChange( bool status, Machine& machine, int event /* = 0 */ ) {
  if ( status ) {
    onup.set( &machine, event );
  } else {
    ondown.set( &machine, event );
  }
  return *this;
}

Atm_encoder& Atm_encoder::onChange( bool status, atm_cb_push_t callback, int idx /* = 0 */ ) {
  if ( status ) {
    onup.set( callback, idx );
  } else {
    ondown.set( callback, idx );
  }
  return *this;
}

int Atm_encoder::state( void ) {
  return range_invert ? map( value, min, max, max, min ) : value;
}

bool Atm_encoder::count( int direction ) {
  if ( (long)value + direction > max ) {
    if ( wrap ) {
      value = min;
    } else {
      return false;
    }
  } else if ( (long)value + direction < min ) {
    if ( wrap ) {
      value = max;
    } else {
      return false;
    }
  } else {
    value += direction;
  }
  return true;
}

Atm_encoder& Atm_encoder::trace( Stream& stream ) {
  Machine::setTrace( &stream, atm_serial_debug::trace, "ENCODER\0EVT_UP\0EVT_DOWN\0ELSE\0IDLE\0UP\0DOWN" );
  return *this;
}
