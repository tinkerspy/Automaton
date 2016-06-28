#include "Atm_player.hpp"

/* Add optional parameters for the state machine to begin()
 * Add extra initialization code
 */

Atm_player& Atm_player::begin( int pin /* = - 1 */ ) {
  // clang-format off
  const static state_t state_table[] PROGMEM = {
    /*             ON_ENTER    ON_LOOP  ON_EXIT  EVT_START  EVT_STOP  EVT_TOGGLE  EVT_TIMER  EVT_EOPAT  EVT_REPEAT   ELSE */
    /*   IDLE */   ENT_IDLE, ATM_SLEEP,      -1,     START,       -1,      START,        -1,        -1,         -1,    -1,
    /*  START */  ENT_START,        -1,      -1,        -1,       -1,         -1,        -1,        -1,         -1, SOUND,
    /*  SOUND */  ENT_SOUND,        -1,      -1,        -1,     IDLE,       IDLE,     QUIET,        -1,         -1,    -1,
    /*  QUIET */  ENT_QUIET,        -1,      -1,        -1,     IDLE,       IDLE,      NEXT,        -1,         -1,    -1,
    /*   NEXT */   ENT_NEXT,        -1,      -1,        -1,     IDLE,       IDLE,        -1,    REPEAT,         -1, SOUND,
    /* REPEAT */ ENT_REPEAT,        -1,      -1,        -1,     IDLE,       IDLE,        -1,        -1,     FINISH, START,
    /* FINISH */ ENT_FINISH,        -1,      -1,        -1,     IDLE,         -1,        -1,        -1,       IDLE, START,
  };
  // clang-format on
  Machine::begin( state_table, ELSE );
  Atm_player::pin = pin;
  speed( 100 );
  pitch( 100 );
  repeat( 1 );
  play( 880, 50 );
  return *this;
}

/* Add C++ code for each event (input)
 * The code must return 1 if the event should be triggered
 */

int Atm_player::event( int id ) {
  switch ( id ) {
    case EVT_START:
      return 0;
    case EVT_TIMER:
      return timer.expired( this );
    case EVT_EOPAT:
      return ( step * 6 ) >= patternsize;
    case EVT_REPEAT:
      return counter_repeat.expired();
  }
  return 0;
}

/* Add C++ code for each action
 * This generates the 'output' for the state machine
 */

void Atm_player::action( int id ) {
  switch ( id ) {
    case ENT_FINISH:
      push( connectors, ON_FINISH, 0, 0, 0 );
      return;
    case ENT_IDLE:
      if ( pin >= 0 ) noTone( pin );  // Tone takes up 7 bytes extra SRAM
      counter_repeat.set( repeatCount );
      return;
    case ENT_START:
      step = 0;
      counter_repeat.decrement();
      return;
    case ENT_SOUND:
      push( connectors, ON_NOTE, true, pattern[step * 3] * pitchFactor, 1 );
      if ( pin >= 0 ) tone( pin, pattern[step * 3] * pitchFactor );
      timer.set( pattern[step * 3 + 1] * speedFactor );
      return;
    case ENT_QUIET:
      push( connectors, ON_NOTE, false, pattern[step * 3] * pitchFactor, 0 );
      if ( pin >= 0 ) noTone( pin );
      timer.set( pattern[step * 3 + 2] * speedFactor );
      return;
    case ENT_NEXT:
      step++;
      return;
    case ENT_REPEAT:
      return;
  }
}

/* How many times to repeat the pattern
 *
 */

Atm_player& Atm_player::repeat( uint16_t v ) {
  counter_repeat.set( repeatCount = v );
  return *this;
}

Atm_player& Atm_player::speed( float v ) {
  speedFactor = 100 / v;
  return *this;
}

Atm_player& Atm_player::pitch( float v ) {
  pitchFactor = v / 100;
  return *this;
}

Atm_player& Atm_player::start( void ) {
  trigger( EVT_START );
  return *this;
}

Atm_player& Atm_player::stop( void ) {
  trigger( EVT_STOP );
  return *this;
}

Atm_player& Atm_player::toggle( void ) {
  trigger( EVT_TOGGLE );
  return *this;
}

/* Sets the pattern and pattern length (in bytes)
 *
 */

Atm_player& Atm_player::play( int* pat, int patsize ) {
  pattern = pat;
  patternsize = patsize;
  counter_repeat.set( repeatCount );
  step = 0;
  return *this;
}

Atm_player& Atm_player::play( int freq, int period, int pause /* = 0 */ ) {
  stub[0] = freq;
  stub[1] = period;
  stub[2] = pause;
  pattern = stub;
  patternsize = 6;
  step = 0;
  return *this;
}

/* Optionally override the default trigger() method
 * Control what triggers your machine can and cannot process
 */

Atm_player& Atm_player::trigger( int event ) {
  Machine::trigger( event );
  return *this;
}

/* Optionally override the default state() method
 * Control what the machine returns when another process requests its state()
 */

int Atm_player::state( void ) {
  return Machine::state();
}

/* Nothing customizable below this line
 ************************************************************************************************
*/

/* onFinish() push connector variants ( slots 1, autostore 0, broadcast 0 )
 *
 * Usage in action() handler: push( connectors, ON_FINISH, 0, v, up)
 */

Atm_player& Atm_player::onFinish( Machine& machine, int event ) {
  onPush( connectors, ON_FINISH, 0, 1, 1, machine, event );
  return *this;
}
Atm_player& Atm_player::onFinish( atm_cb_push_t callback, int idx ) {
  onPush( connectors, ON_FINISH, 0, 1, 1, callback, idx );
  return *this;
}

/* onNote() push connector variants ( slots 2, autostore 0, broadcast 0 )
 *
 * Usage in action() handler: push( connectors, ON_NOTE, sub, v, up)
 */

Atm_player& Atm_player::onNote( Machine& machine, int event ) {
  onPush( connectors, ON_NOTE, 0, 2, 1, machine, event );
  return *this;
}
Atm_player& Atm_player::onNote( atm_cb_push_t callback, int idx ) {
  onPush( connectors, ON_NOTE, 0, 2, 1, callback, idx );
  return *this;
}
Atm_player& Atm_player::onNote( int sub, Machine& machine, int event ) {
  onPush( connectors, ON_NOTE, sub, 2, 0, machine, event );
  return *this;
}
Atm_player& Atm_player::onNote( int sub, atm_cb_push_t callback, int idx ) {
  onPush( connectors, ON_NOTE, sub, 2, 0, callback, idx );
  return *this;
}

/* State trace method
 * Sets the symbol table and the default logging method for serial monitoring
 */

Atm_player& Atm_player::trace( Stream& stream ) {
  Machine::setTrace( &stream, atm_serial_debug::trace,
                     "PLAYER\0EVT_START\0EVT_STOP\0EVT_TOGGLE\0EVT_TIMER\0EVT_EOPAT\0EVT_REPEAT\0ELSE\0IDLE\0START\0SOUND\0QUIET\0NEXT\0REPEAT\0FINISH" );
  return *this;
}
