#include "Atm_controller.hpp"

const char Atm_controller::_relOps[8] = "0=!<>-+";

Atm_controller& Atm_controller::begin( bool initialState /* = false */ ) {
  // clang-format off
  const static state_t state_table[] PROGMEM = {
    /*              ON_ENTER    ON_LOOP  ON_EXIT  EVT_ON  EVT_OFF  EVT_INPUT ELSE */
    /* OFF     */    ACT_OFF,        -1,      -1,     ON,      -1,       OFF,  -1,
    /* ON      */     ACT_ON,        -1,      -1,     -1,     OFF,        ON,  -1,
  };
  // clang-format on
  Machine::begin( state_table, ELSE );
  _last_state = -1;
  state( initialState ? ON : OFF );
  _indicator = -1;
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
    case ACT_OFF:
      _connector[_last_state == current ? ON_INPUT_FALSE : ON_CHANGE_FALSE].push();
      if ( _indicator > -1 ) digitalWrite( _indicator, !LOW != !_indicatorActiveLow );
      _last_state = current;
      return;
    case ACT_ON:
      if ( _last_state != -1 ) _connector[( _last_state == current ) ? ON_INPUT_TRUE : ON_CHANGE_TRUE].push();
      if ( _indicator > -1 ) digitalWrite( _indicator, !HIGH != !_indicatorActiveLow );
      _last_state = current;
      return;
  }
}

bool Atm_controller::eval_one( atm_connector& connector ) {
  switch ( connector.relOp() ) {
    case connector.REL_EQ:
      return connector.pull() == connector.event;
    case connector.REL_NEQ:
      return connector.pull() != connector.event;
    case connector.REL_LT:
      return connector.pull() < connector.event;
    case connector.REL_GT:
      return connector.pull() > connector.event;
    case connector.REL_LTE:
      return connector.pull() <= connector.event;
    case connector.REL_GTE:
      return connector.pull() >= connector.event;
  }
  return false;
}

bool Atm_controller::eval_all() {
  bool r = eval_one( _operand[0] );
  for ( uint8_t i = 1; i < ATM_CONDITION_OPERAND_MAX; i++ ) {
    if ( _operand[i].mode() ) {
      switch ( _operand[i].logOp() ) {
        case atm_connector::LOG_AND:
          r = r && eval_one( _operand[i] );
          break;
        case atm_connector::LOG_OR:
          r = r || eval_one( _operand[i] );
          break;
        case atm_connector::LOG_XOR:
          r = !r != !eval_one( _operand[i] );
          break;
      }
    }
  }
  return r;
}

Atm_controller& Atm_controller::led( int led, bool activeLow /* = false */ ) {
  _indicator = led;
  _indicatorActiveLow = activeLow;
  pinMode( _indicator, OUTPUT );
  return *this;
}

Atm_controller& Atm_controller::onChange( bool status, atm_cb_push_t callback, int idx /* = 0 */ ) {
  _connector[status ? ON_CHANGE_TRUE : ON_CHANGE_FALSE].set( callback, idx );
  return *this;
}

Atm_controller& Atm_controller::onChange( bool status, Machine& machine, int event /* = 0 */ ) {
  _connector[status ? ON_CHANGE_TRUE : ON_CHANGE_FALSE].set( &machine, event );
  return *this;
}

Atm_controller& Atm_controller::onChange( atm_cb_push_t callback, int idx /* = 0 */ ) {
  _connector[ON_CHANGE_TRUE].set( callback, idx );
  _connector[ON_CHANGE_FALSE].set( callback, idx );
  return *this;
}

Atm_controller& Atm_controller::onChange( Machine& machine, int event /* = 0 */ ) {
  _connector[ON_CHANGE_TRUE].set( &machine, event );
  _connector[ON_CHANGE_FALSE].set( &machine, event );
  return *this;
}

Atm_controller& Atm_controller::onInput( bool status, atm_cb_push_t callback, int idx /* = 0 */ ) {
  _connector[status ? ON_INPUT_TRUE : ON_INPUT_FALSE].set( callback, idx );
  return *this;
}

Atm_controller& Atm_controller::onInput( bool status, Machine& machine, int event /* = 0 */ ) {
  _connector[status ? ON_INPUT_TRUE : ON_INPUT_FALSE].set( &machine, event );
  return *this;
}

Atm_controller& Atm_controller::IF( Machine& machine, char relOp /* = '>' */, int match /* = 0 */ ) {
  return OP( atm_connector::LOG_AND, machine, relOp, match );
}

Atm_controller& Atm_controller::IF( atm_cb_pull_t callback, char relOp /* = '>' */, int match /* = 0 */ ) {
  return OP( atm_connector::LOG_AND, callback, relOp, match );
}

Atm_controller& Atm_controller::AND( Machine& machine, char relOp /* = '>' */, int match /* = 0 */ ) {
  return OP( atm_connector::LOG_AND, machine, relOp, match );
}

Atm_controller& Atm_controller::AND( atm_cb_pull_t callback, char relOp /* = '>' */, int match /* = 0 */ ) {
  return OP( atm_connector::LOG_AND, callback, relOp, match );
}

Atm_controller& Atm_controller::OR( Machine& machine, char relOp /* = '>' */, int match /* = 0 */ ) {
  return OP( atm_connector::LOG_OR, machine, relOp, match );
}

Atm_controller& Atm_controller::OR( atm_cb_pull_t callback, char relOp /* = '>' */, int match /* = 0 */ ) {
  return OP( atm_connector::LOG_OR, callback, relOp, match );
}

Atm_controller& Atm_controller::XOR( Machine& machine, char relOp /* = '>' */, int match /* = 0 */ ) {
  return OP( atm_connector::LOG_XOR, machine, relOp, match );
}

Atm_controller& Atm_controller::XOR( atm_cb_pull_t callback, char relOp /* = '>' */, int match /* = 0 */ ) {
  return OP( atm_connector::LOG_XOR, callback, relOp, match );
}

Atm_controller& Atm_controller::OP( char logOp, Machine& machine, char relOp, int match ) {
  for ( uint8_t i = 0; i < ATM_CONDITION_OPERAND_MAX; i++ ) {
    if ( _operand[i].mode() == atm_connector::MODE_NULL ) {  // Pick the first free slot
      _operand[i].set( &machine, match, logOp, (int)( strchr( _relOps, relOp ) - _relOps ) );
      break;
    }
  }
  return *this;
}

Atm_controller& Atm_controller::OP( char logOp, atm_cb_pull_t callback, char relOp, int match ) {
  for ( uint8_t i = 0; i < ATM_CONDITION_OPERAND_MAX; i++ ) {
    if ( _operand[i].mode() == atm_connector::MODE_NULL ) {  // Pick the first free slot
      _operand[i].set( callback, match, logOp, (int)( strchr( _relOps, relOp ) - _relOps ) );
      break;
    }
  }
  return *this;
}

Atm_controller& Atm_controller::trace( Stream& stream ) {
  Machine::setTrace( &stream, atm_serial_debug::trace, "CONTROLLER\0EVT_ON\0EVT_OFF\0EVT_TOGGLE\0EVT_INPUT\0ELSE\0OFF\0ON" );
  return *this;
}
