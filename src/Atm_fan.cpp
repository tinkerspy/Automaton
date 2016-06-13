
#include "Atm_fan.hpp"

/* Add optional parameters for the state machine to begin()
 * Add extra initialization code
 */

Atm_fan& Atm_fan::begin() {
  // clang-format off
  const static state_t state_table[] PROGMEM = {
    /*          ON_ENTER    ON_LOOP  ON_EXIT  EVT_INPUT  ELSE */
    /*  IDLE */       -1, ATM_SLEEP,      -1,      SEND,   -1,
    /*  SEND */ ENT_SEND,        -1,      -1,        -1, IDLE,
  };
  // clang-format on
  Machine::begin( state_table, ELSE );
  return *this;
}

/* Add C++ code for each internally handled event (input)
 * The code must return 1 to trigger the event
 */

int Atm_fan::event( int id ) {
  switch ( id ) {
    case EVT_INPUT:
      return 0;
  }
  return 0;
}

/* Add C++ code for each action
 * This generates the 'output' for the state machine
 */

void Atm_fan::action( int id ) {
  switch ( id ) {
    case ENT_SEND:
      push( connectors, ON_INPUT | ATM_BROADCAST, 4, 1, 1 );
      return;
  }
}

/* Optionally override the default trigger() method
 * Control how your machine processes triggers
 */

Atm_fan& Atm_fan::trigger( int event ) {
  Machine::trigger( event );
  return *this;
}

/* Optionally override the default state() method
 * Control what the machine returns when another process requests its state
 */

int Atm_fan::state( void ) {
  return Machine::state();
}

/* Nothing customizable below this line
 ************************************************************************************************
*/

/* onInput() push connector variants ( slots 4, autostore 1, broadcast 1 )
 *
 * Usage in action() handler: push( connectors, ON_INPUT | ATM_BROADCAST, 4, v, up );
 */

Atm_fan& Atm_fan::onInput( Machine& machine, int event ) {
  onPush( connectors, ON_INPUT, -1, 4, 0, machine, event );
  return *this;
}

Atm_fan& Atm_fan::onInput( atm_cb_push_t callback, int idx ) {
  onPush( connectors, ON_INPUT, -1, 4, 0, callback, idx );
  return *this;
}

/* State trace method
 * Sets the symbol table and the default logging method for serial monitoring
 */

Atm_fan& Atm_fan::trace( Stream& stream ) {
  Machine::setTrace( &stream, atm_serial_debug::trace, "FAN\0EVT_INPUT\0ELSE\0IDLE\0SEND" );
  return *this;
}
