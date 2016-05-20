#include "Atm_controller.hpp"

const char Atm_controller::relOps[8] = "0=!<>-+";

Atm_controller& Atm_controller::begin( bool initialState /* = false */ ) {
  // clang-format off
  const static state_t state_table[] PROGMEM = {
    /*              ON_ENTER    ON_LOOP  ON_EXIT  EVT_ON  EVT_OFF  EVT_TOGGLE EVT_INPUT ELSE */
    /* OFF     */    ACT_OFF,        -1,      -1,     ON,      -1,         ON,      OFF,  -1,
    /* ON      */     ACT_ON,        -1,      -1,     -1,     OFF,        OFF,       ON,  -1,
  };
  // clang-format on
  Machine::begin( state_table, ELSE );
  _last_state = -1;
  state( initialState ? ON : OFF );
  _indicator = -1;
  return *this;
}

Atm_controller& Atm_controller::indicator( int led, bool activeLow /* = false */ ) {
  _indicator = led;
  _indicatorActiveLow = activeLow;
  pinMode( _indicator, OUTPUT );
  return *this;
}

Atm_controller& Atm_controller::onFlip( bool status, atm_cb_t callback, int idx /* = 0 */ ) {
  _connector[status ? 0 : 1].set( callback, idx );
  return *this;
}

Atm_controller& Atm_controller::onFlip( bool status, Machine& machine, int evt /* = 0 */ ) {
  _connector[status ? 0 : 1].set( &machine, evt );
  return *this;
}

Atm_controller& Atm_controller::onInput( bool status, atm_cb_t callback, int idx /* = 0 */ ) {
  _connector[status ? 2 : 3].set( callback, idx );
  return *this;
}

Atm_controller& Atm_controller::onInput( bool status, Machine& machine, int event /* = 0 */ ) {
  _connector[status ? 2 : 3].set( &machine, event );
  return *this;
}

Atm_controller& Atm_controller::IF( Machine& machine, char relOp /* = '>' */, int match /* = 0 */ ) {
  return OP( atm_connector::LOG_AND, machine, relOp, match );
}

Atm_controller& Atm_controller::IF( atm_cb_t callback, char relOp /* = '>' */, int match /* = 0 */ ) {
  return OP( atm_connector::LOG_AND, callback, relOp, match );
}

Atm_controller& Atm_controller::AND( Machine& machine, char relOp /* = '>' */, int match /* = 0 */ ) {
  return OP( atm_connector::LOG_AND, machine, relOp, match );
}

Atm_controller& Atm_controller::AND( atm_cb_t callback, char relOp /* = '>' */, int match /* = 0 */ ) {
  return OP( atm_connector::LOG_AND, callback, relOp, match );
}

Atm_controller& Atm_controller::OR( Machine& machine, char relOp /* = '>' */, int match /* = 0 */ ) {
  return OP( atm_connector::LOG_OR, machine, relOp, match );
}

Atm_controller& Atm_controller::OR( atm_cb_t callback, char relOp /* = '>' */, int match /* = 0 */ ) {
  return OP( atm_connector::LOG_OR, callback, relOp, match );
}

Atm_controller& Atm_controller::XOR( Machine& machine, char relOp /* = '>' */, int match /* = 0 */ ) {
  return OP( atm_connector::LOG_XOR, machine, relOp, match );
}

Atm_controller& Atm_controller::XOR( atm_cb_t callback, char relOp /* = '>' */, int match /* = 0 */ ) {
  return OP( atm_connector::LOG_XOR, callback, relOp, match );
}

Atm_controller& Atm_controller::OP( char logOp, Machine& machine, char relOp, int match ) {
  for ( uint8_t i = 0; i < ATM_CONDITION_OPERAND_MAX; i++ ) {
    if ( _operand[i].mode() == atm_connector::MODE_NULL ) {  // Pick the first free slot
      _operand[i].set( &machine, match, logOp, (int)( strchr( relOps, relOp ) - relOps ) );
      break;
    }
  }
  return *this;
}

Atm_controller& Atm_controller::OP( char logOp, atm_cb_t callback, char relOp, int match ) {
  for ( uint8_t i = 0; i < ATM_CONDITION_OPERAND_MAX; i++ ) {
    if ( _operand[i].mode() == atm_connector::MODE_NULL ) {  // Pick the first free slot
      _operand[i].set( callback, match, logOp, (int)( strchr( relOps, relOp ) - relOps ) );
      break;
    }
  }
  return *this;
}

bool Atm_controller::eval_one( uint8_t idx ) {
  switch ( _operand[idx].relOp() ) {
    case atm_connector::REL_EQ:
      return _operand[idx].pull() == _operand[idx].event;
    case atm_connector::REL_NEQ:
      return _operand[idx].pull() != _operand[idx].event;
    case atm_connector::REL_LT:
      return _operand[idx].pull() < _operand[idx].event;
    case atm_connector::REL_GT:
      return _operand[idx].pull() > _operand[idx].event;
    case atm_connector::REL_LTE:
      return _operand[idx].pull() <= _operand[idx].event;
    case atm_connector::REL_GTE:
      return _operand[idx].pull() >= _operand[idx].event;
  }
  return false;
}

bool Atm_controller::eval_all() {
  bool r = eval_one( 0 );
  for ( uint8_t i = 1; i < ATM_CONDITION_OPERAND_MAX; i++ ) {
    if ( _operand[i].mode() ) {
      switch ( _operand[i].logOp() ) {
        case atm_connector::LOG_AND:
          r = r && eval_one( i );
          break;
        case atm_connector::LOG_OR:
          r = r || eval_one( i );
          break;
        case atm_connector::LOG_XOR:
          r = !r != !eval_one( i );
          break;
      }
    }
  }
  return r;
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
      _connector[_last_state == current ? 3 : 1].push();
      if ( _indicator > -1 ) digitalWrite( _indicator, !LOW != !_indicatorActiveLow );
      _last_state = current;
      return;
    case ACT_ON:
      if ( _last_state != -1 ) _connector[( _last_state == current ) ? 2 : 0].push();
      if ( _indicator > -1 ) digitalWrite( _indicator, !HIGH != !_indicatorActiveLow );
      _last_state = current;
      return;
  }
}

Atm_controller& Atm_controller::trace( Stream& stream ) {
  Machine::setTrace( &stream, atm_serial_debug::trace, "EVT_ON\0EVT_OFF\0EVT_TOGGLE\0EVT_INPUT\0ELSE\0OFF\0ON" );
  return *this;
}
