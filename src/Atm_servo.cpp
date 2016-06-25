#include "Atm_servo.hpp"

/* Add optional parameters for the state machine to begin()
 * Add extra initialization code
 */

Atm_servo& Atm_servo::begin( int pin, int pos ) {
  // clang-format off
  const static state_t state_table[] PROGMEM = {
    /*                  ON_ENTER    ON_LOOP  ON_EXIT  EVT_MVUP  EVT_MVDN  EVT_TIMER      ELSE */
    /*      IDLE */     ENT_IDLE,        -1,      -1,       UP,     DOWN,        -1,       -1,
    /*        UP */       ENT_UP,        -1,      -1,       -1,       -1,   UP_NEXT,       -1,
    /*   UP_NEXT */           -1,        -1,      -1,       UP,     DOWN,        -1, FINISHED,
    /*      DOWN */     ENT_DOWN,        -1,      -1,       -1,       -1, DOWN_NEXT,       -1,
    /* DOWN_NEXT */           -1,        -1,      -1,       UP,     DOWN,        -1, FINISHED,
    /*  FINISHED */ ENT_FINISHED,        -1,      -1,       -1,       -1,        -1,     IDLE,
  };
  // clang-format on
  Machine::begin( state_table, ELSE );
  this->pin = pin;
  servo.attach( pin );
  servo_pos = pos;
  servo_dest = pos;
  step_size = 180;
  timer.set( step_time = 0 );
  servo.write( servo_dest );
  return *this;
}

/* Add C++ code for each internally handled event (input)
 * The code must return 1 to trigger the event
 */

int Atm_servo::event( int id ) {
  switch ( id ) {
    case EVT_MVUP:
      return servo_dest > servo_pos;
    case EVT_MVDN:
      return servo_dest < servo_pos;
    case EVT_TIMER:
      return timer.expired( this );
  }
  return 0;
}

/* Add C++ code for each action
 * This generates the 'output' for the state machine
 */

void Atm_servo::action( int id ) {
  switch ( id ) {
    case ENT_IDLE:
      return;
    case ENT_UP:
      push( connectors, ON_CHANGE, true, servo_pos, 1 );
      servo_pos += step_size;
      if ( servo_pos > servo_dest ) servo_pos = servo_dest;
      servo.write( servo_pos );
      timer.set( step_time );
      return;
    case ENT_DOWN:
      push( connectors, ON_CHANGE, false, servo_pos, 0 );
      servo_pos -= step_size;
      if ( servo_pos < servo_dest ) servo_pos = servo_dest;
      servo.write( servo_pos );
      timer.set( step_time );
      return;
    case ENT_FINISHED:
      push( connectors, ON_FINISH, 0, servo_pos, 0 );
      return;
  }
}

Atm_servo& Atm_servo::step( int degrees, int time ) {
  step_size = degrees;
  timer.set( step_time = time );
  return *this;
}

Atm_servo& Atm_servo::position( int pos ) {
  servo_dest = pos;
  return *this;
}

int Atm_servo::position( void ) {
  return servo_pos;
}

/* Optionally override the default trigger() method
 * Control how your machine processes triggers
 */

Atm_servo& Atm_servo::trigger( int event ) {
  switch ( event ) {
    case EVT_UP:
      servo_dest += step_size;
      if ( servo_dest > 180 ) servo_dest = 180;
      break;
    case EVT_DOWN:
      servo_dest -= step_size;
      if ( servo_dest < 0 ) servo_dest = 0;
      break;
    default:
      servo_dest = event;
      break;
  }
  return *this;
}

/* Optionally override the default state() method
 * Control what the machine returns when another process requests its state
 */

int Atm_servo::state( void ) {
  return Machine::state();
}

/* Nothing customizable below this line
 ************************************************************************************************
*/

/* onChange() push connector variants ( slots 2, autostore 0, broadcast 0 )
 *
 * Usage in action() handler: push( connectors, ON_CHANGE, sub, v, up );
 */

Atm_servo& Atm_servo::onChange( Machine& machine, int event ) {
  onPush( connectors, ON_CHANGE, 0, 2, 1, machine, event );
  return *this;
}
Atm_servo& Atm_servo::onChange( atm_cb_push_t callback, int idx ) {
  onPush( connectors, ON_CHANGE, 0, 2, 1, callback, idx );
  return *this;
}
Atm_servo& Atm_servo::onChange( int sub, Machine& machine, int event ) {
  onPush( connectors, ON_CHANGE, sub, 2, 0, machine, event );
  return *this;
}
Atm_servo& Atm_servo::onChange( int sub, atm_cb_push_t callback, int idx ) {
  onPush( connectors, ON_CHANGE, sub, 2, 0, callback, idx );
  return *this;
}

/* onFinish() push connector variants ( slots 1, autostore 0, broadcast 0 )
 *
 * Usage in action() handler: push( connectors, ON_FINISH, 0, v, up );
 */

Atm_servo& Atm_servo::onFinish( Machine& machine, int event ) {
  onPush( connectors, ON_FINISH, 0, 1, 1, machine, event );
  return *this;
}
Atm_servo& Atm_servo::onFinish( atm_cb_push_t callback, int idx ) {
  onPush( connectors, ON_FINISH, 0, 1, 1, callback, idx );
  return *this;
}

/* State trace method
 * Sets the symbol table and the default logging method for serial monitoring
 */

Atm_servo& Atm_servo::trace( Stream& stream ) {
  Machine::setTrace( &stream, atm_serial_debug::trace, "SERVO\0EVT_MVUP\0EVT_MVDN\0EVT_TIMER\0ELSE\0IDLE\0UP\0UP_NEXT\0DOWN\0DOWN_NEXT\0FINISHED" );
  return *this;
}
