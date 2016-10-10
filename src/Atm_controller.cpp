#include "Atm_controller.hpp"

const char Atm_controller::relOps[8] = "0=!<>-+";

Atm_controller& Atm_controller::begin( bool initialState /* = false */ ) {
  // clang-format off
  const static state_t state_table[] PROGMEM = {
    /*              ON_ENTER    ON_LOOP  ON_EXIT  EVT_ON  EVT_OFF  EVT_INPUT ELSE */
    /* OFF     */    ENT_OFF,        -1,      -1,     ON,      -1,       OFF,  -1,
    /* ON      */     ENT_ON,        -1,      -1,     -1,     OFF,        ON,  -1,
  };
  // clang-format on
  Machine::begin( state_table, ELSE );
  last_state = -1;
  state( initialState ? ON : OFF );
  indicator = -1;
  return *this;
}

int Atm_controller::event( int id ) {
  switch ( id ) {
    case EVT_ON:
      return eval_all();
    case EVT_OFF:
      return !eval_all();
  }
  return 0;
}

void Atm_controller::action( int id ) {
  switch ( id ) {
    case ENT_OFF:
      connector[last_state == current ? ON_INPUT_FALSE : ON_CHANGE_FALSE].push( state() );
      if ( indicator > -1 ) digitalWrite( indicator, !LOW != !indicatorActiveLow );
      last_state = current;
      return;
    case ENT_ON:
      if ( last_state != -1 ) connector[( last_state == current ) ? ON_INPUT_TRUE : ON_CHANGE_TRUE].push( state() );
      if ( indicator > -1 ) digitalWrite( indicator, !HIGH != !indicatorActiveLow );
      last_state = current;
      return;
  }
}

bool Atm_controller::eval_one( atm_connector& connector ) {
  switch ( connector.relOp() ) {
    case atm_connector::REL_EQ:
      return connector.pull() == connector.event;
    case atm_connector::REL_NEQ:
      return connector.pull() != connector.event;
    case atm_connector::REL_LT:
      return connector.pull() < connector.event;
    case atm_connector::REL_GT:
      return connector.pull() > connector.event;
    case atm_connector::REL_LTE:
      return connector.pull() <= connector.event;
    case atm_connector::REL_GTE:
      return connector.pull() >= connector.event;
  }
  return connector.pull();
}

bool Atm_controller::eval_all() {
  bool r = eval_one( operand[0] );
  for ( uint8_t i = 1; i < ATM_CONDITION_OPERAND_MAX; i++ ) {
    if ( operand[i].mode() ) {
      switch ( operand[i].logOp() ) {
        case atm_connector::LOG_AND:
          r = r && eval_one( operand[i] );
          break;
        case atm_connector::LOG_OR:
          r = r || eval_one( operand[i] );
          break;
        case atm_connector::LOG_XOR:
          r = !r != !eval_one( operand[i] );
          break;
      }
    }
  }
  return r;
}

Atm_controller& Atm_controller::led( int led, bool activeLow /* = false */ ) {
  indicator = led;
  indicatorActiveLow = activeLow;
  pinMode( indicator, OUTPUT );
  return *this;
}

Atm_controller& Atm_controller::onChange( bool status, atm_cb_push_t callback, int idx /* = 0 */ ) {
  connector[status ? ON_CHANGE_TRUE : ON_CHANGE_FALSE].set( callback, idx );
  return *this;
}

Atm_controller& Atm_controller::onChange( bool status, Machine& machine, int event /* = 0 */ ) {
  connector[status ? ON_CHANGE_TRUE : ON_CHANGE_FALSE].set( &machine, event );
  return *this;
}

Atm_controller& Atm_controller::onChange( atm_cb_push_t callback, int idx /* = 0 */ ) {
  connector[ON_CHANGE_TRUE].set( callback, idx );
  connector[ON_CHANGE_FALSE].set( callback, idx );
  return *this;
}

Atm_controller& Atm_controller::onChange( Machine& machine, int event /* = 0 */ ) {
  connector[ON_CHANGE_TRUE].set( &machine, event );
  connector[ON_CHANGE_FALSE].set( &machine, event );
  return *this;
}

Atm_controller& Atm_controller::onInput( bool status, atm_cb_push_t callback, int idx /* = 0 */ ) {
  connector[status ? ON_INPUT_TRUE : ON_INPUT_FALSE].set( callback, idx );
  return *this;
}

Atm_controller& Atm_controller::onInput( bool status, Machine& machine, int event /* = 0 */ ) {
  connector[status ? ON_INPUT_TRUE : ON_INPUT_FALSE].set( &machine, event );
  return *this;
}

Atm_controller& Atm_controller::IF( Machine& machine, char relOp /* = '>' */, int match /* = 0 */ ) {
  return OP( atm_connector::LOG_AND, machine, relOp, match );
}

Atm_controller& Atm_controller::IF( atm_cb_pull_t callback, int idx /* = 0 */ ) {
  return OP( atm_connector::LOG_AND, callback, idx );
}

Atm_controller& Atm_controller::AND( Machine& machine, char relOp /* = '>' */, int match /* = 0 */ ) {
  return OP( atm_connector::LOG_AND, machine, relOp, match );
}

Atm_controller& Atm_controller::AND( atm_cb_pull_t callback, int idx /* = 0 */ ) {
  return OP( atm_connector::LOG_AND, callback, idx );
}

Atm_controller& Atm_controller::OR( Machine& machine, char relOp /* = '>' */, int match /* = 0 */ ) {
  return OP( atm_connector::LOG_OR, machine, relOp, match );
}

Atm_controller& Atm_controller::OR( atm_cb_pull_t callback, int idx /* = 0 */ ) {
  return OP( atm_connector::LOG_OR, callback, idx );
}

Atm_controller& Atm_controller::XOR( Machine& machine, char relOp /* = '>' */, int match /* = 0 */ ) {
  return OP( atm_connector::LOG_XOR, machine, relOp, match );
}

Atm_controller& Atm_controller::XOR( atm_cb_pull_t callback, int idx /* = 0 */ ) {
  return OP( atm_connector::LOG_XOR, callback, idx );
}

Atm_controller& Atm_controller::OP( char logOp, Machine& machine, char relOp, int match ) {
  for ( uint8_t i = 0; i < ATM_CONDITION_OPERAND_MAX; i++ ) {
    if ( operand[i].mode() == atm_connector::MODE_NULL ) {  // Pick the first free slot
      operand[i].set( &machine, match, logOp, (int)( strchr( relOps, relOp ) - relOps ) );
      break;
    }
  }
  return *this;
}

Atm_controller& Atm_controller::OP( char logOp, atm_cb_pull_t callback, int idx ) {
  for ( uint8_t i = 0; i < ATM_CONDITION_OPERAND_MAX; i++ ) {
    if ( operand[i].mode() == atm_connector::MODE_NULL ) {  // Pick the first free slot
      operand[i].set( callback, idx );
      break;
    }
  }
  return *this;
}

Atm_controller& Atm_controller::trace( Stream& stream ) {
  Machine::setTrace( &stream, atm_serial_debug::trace, "CONTROLLER\0EVT_ON\0EVT_OFF\0EVT_INPUT\0ELSE\0OFF\0ON" );
  return *this;
}
