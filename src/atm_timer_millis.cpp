
#include "Automaton.h"

/*
 * atm_timer_millis::set( v ) - Sets a millis timer to 'v'
 *
 */

void atm_timer_millis::set( uint32_t v ) {
  value = v;
}

/*
 * atm_timer_millis::setFromNow( Machine* machine, uint32_t v ) -
 * Sets the timer to be expired v millis from now
 */
void atm_timer_millis::setFromNow( Machine* machine, uint32_t v ) {
  value = millis() - machine->state_millis + v;
}

/*
 * atm_timer_millis::elapsed( this ) - returns the elapsed time so far
 * This is a rollover-safe 32 bit unsigned integer
 *
 */
uint32_t atm_timer_millis::elapsed( Machine* machine ) {
    return value == ATM_TIMER_OFF ? 0 : millis() - machine->state_millis;
}

/*
 * atm_timer_millis::expired( this ) - Checks a millis timer for expiry (== 0)
 * This is a rollover-safe 32 bit unsigned integer comparison
 *
 */

int atm_timer_millis::expired( Machine* machine ) {
  return value == ATM_TIMER_OFF ? 0 : millis() - machine->state_millis >= value;
}
