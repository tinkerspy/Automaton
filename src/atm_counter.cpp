/*
  Automaton - Reactive State Machine Framework for Arduino.
  Published under the MIT License (MIT), Copyright (c) 2015-2016, J.P. van der Landen
*/

#include "Automaton.h"

/*
 * atm_counter::set( v ) - Sets a countdown counter to 'v'
 *
 */

void atm_counter::set( uint16_t v ) {
  value = v;
}

/*
 * atm_counter::decrement() - Decrements a countdown counter
 *
 */

uint16_t atm_counter::decrement( void ) {
  return value > 0 && value != ATM_COUNTER_OFF ? --value : 0;
}

/*
 * atm_counter::expired() - Checks a countdown counter for expiry (== 0)
 *
 */

uint8_t atm_counter::expired() {
  return value == ATM_COUNTER_OFF ? 0 : ( value > 0 ? 0 : 1 );
}
