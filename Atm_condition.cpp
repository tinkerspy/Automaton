#include "Atm_condition.hpp"

#define FACTORY factory
#define STATE_TYPE state_t
#define MACHINE Machine
#undef TINYMACHINE

Atm_condition& Atm_condition::begin( bool default_state /* = false */ ) {
  // clang-format off
  const static STATE_TYPE state_table[] PROGMEM = {
    /*              ON_ENTER    ON_LOOP  ON_EXIT  EVT_ON  EVT_OFF  EVT_TOGGLE EVT_INPUT ELSE */
    /* OFF     */    ACT_OFF,        -1,      -1,     ON,      -1,         ON,      OFF,  -1,
    /* ON      */     ACT_ON,        -1,      -1,     -1,     OFF,        OFF,       ON,  -1,
  };
  // clang-format on
  MACHINE::begin( state_table, ELSE );
  _last_state = -1;
  state( default_state ? ON : OFF );
  return *this;
}

const char Atm_condition::relOps[] = "0=!<>-+";

Atm_condition& Atm_condition::onFlip( bool st, atm_cb_t callback, int16_t idx /* = 0 */ ) {
  _connection[st ? 0 : 1].set( callback, idx );
  return *this;
}

Atm_condition& Atm_condition::onFlip( bool st, Machine& machine, int16_t evt /* = 0 */ ) {
  _connection[st ? 0 : 1].set( &machine, evt );
  return *this;
}

Atm_condition& Atm_condition::onFlip( bool st, const char* label, int16_t event /* = 0 */ ) {
  _connection[st ? 0 : 1].set( label, event );
  return *this;
}

Atm_condition& Atm_condition::onFlip( bool st, TinyMachine& machine, int16_t event /* = 0 */ ) {
  _connection[st ? 0 : 1].set( &machine, event );
  return *this;
}

Atm_condition& Atm_condition::onInput( bool st, atm_cb_t callback, int16_t idx /* = 0 */ ) {
  _connection[st ? 2 : 3].set( callback, idx );
  return *this;
}

Atm_condition& Atm_condition::onInput( bool st, Machine& machine, state_t event /* = 0 */ ) {
  _connection[st ? 2 : 3].set( &machine, event );
  return *this;
}

Atm_condition& Atm_condition::onInput( bool st, const char* label, state_t event /* = 0 */ ) {
  _connection[st ? 2 : 3].set( label, event );
  return *this;
}

Atm_condition& Atm_condition::onInput( bool st, TinyMachine& machine, state_t event /* = 0 */ ) {
  _connection[st ? 2 : 3].set( &machine, event );
  return *this;
}

Atm_condition& Atm_condition::IF( Machine& machine, char relOp /* = '>' */, state_t match /* = 0 */ ) {
  return OP( atm_connector::LOG_AND, machine, relOp, match );
}

Atm_condition& Atm_condition::IF( TinyMachine& machine, char relOp /* = '>' */, state_t match /* = 0 */ ) {
  return OP( atm_connector::LOG_AND, machine, relOp, match );
}

Atm_condition& Atm_condition::IF( const char* label, char relOp /* = '>' */, state_t match /* = 0 */ ) {
  return OP( atm_connector::LOG_AND, label, relOp, match );
}

Atm_condition& Atm_condition::IF( atm_cb_t callback, char relOp /* = '>' */, state_t match /* = 0 */ ) {
  return OP( atm_connector::LOG_AND, callback, relOp, match );
}

Atm_condition& Atm_condition::AND( Machine& machine, char relOp /* = '>' */, state_t match /* = 0 */ ) {
  return OP( atm_connector::LOG_AND, machine, relOp, match );
}

Atm_condition& Atm_condition::AND( TinyMachine& machine, char relOp /* = '>' */, state_t match /* = 0 */ ) {
  return OP( atm_connector::LOG_AND, machine, relOp, match );
}

Atm_condition& Atm_condition::AND( const char* label, char relOp /* = '>' */, state_t match /* = 0 */ ) {
  return OP( atm_connector::LOG_AND, label, relOp, match );
}

Atm_condition& Atm_condition::AND( atm_cb_t callback, char relOp /* = '>' */, state_t match /* = 0 */ ) {
  return OP( atm_connector::LOG_AND, callback, relOp, match );
}

Atm_condition& Atm_condition::OR( Machine& machine, char relOp /* = '>' */, state_t match /* = 0 */ ) {
  return OP( atm_connector::LOG_OR, machine, relOp, match );
}

Atm_condition& Atm_condition::OR( TinyMachine& machine, char relOp /* = '>' */, state_t match /* = 0 */ ) {
  return OP( atm_connector::LOG_OR, machine, relOp, match );
}

Atm_condition& Atm_condition::OR( const char* label, char relOp /* = '>' */, state_t match /* = 0 */ ) {
  return OP( atm_connector::LOG_OR, label, relOp, match );
}

Atm_condition& Atm_condition::OR( atm_cb_t callback, char relOp /* = '>' */, state_t match /* = 0 */ ) {
  return OP( atm_connector::LOG_OR, callback, relOp, match );
}

Atm_condition& Atm_condition::OP( char logOp, Machine& machine, char relOp, state_t match ) {
  for ( uint8_t i = 0; i < ATM_CONDITION_OPERAND_MAX; i++ ) {
    if ( _operand[i].mode() == atm_connector::MODE_NULL ) {  // Pick the first free slot
      _operand[i].set( &machine, match, logOp, (int)( strchr( relOps, relOp ) - relOps ) );
      break;
    }
  }
  return *this;
}

Atm_condition& Atm_condition::OP( char logOp, TinyMachine& machine, char relOp, state_t match ) {
  for ( uint8_t i = 0; i < ATM_CONDITION_OPERAND_MAX; i++ ) {
    if ( _operand[i].mode() == atm_connector::MODE_NULL ) {  // Pick the first free slot
      _operand[i].set( &machine, match, logOp, (int)( strchr( relOps, relOp ) - relOps ) );
      break;
    }
  }
  return *this;
}

Atm_condition& Atm_condition::OP( char logOp, const char* label, char relOp, state_t match ) {
  for ( uint8_t i = 0; i < ATM_CONDITION_OPERAND_MAX; i++ ) {
    if ( _operand[i].mode() == atm_connector::MODE_NULL ) {  // Pick the first free slot
      _operand[i].set( label, match, logOp, (int)( strchr( relOps, relOp ) - relOps ) );
      break;
    }
  }
  return *this;
}

Atm_condition& Atm_condition::OP( char logOp, atm_cb_t callback, char relOp, state_t match ) {
  for ( uint8_t i = 0; i < ATM_CONDITION_OPERAND_MAX; i++ ) {
    if ( _operand[i].mode() == atm_connector::MODE_NULL ) {  // Pick the first free slot
      _operand[i].set( callback, match, logOp, (int)( strchr( relOps, relOp ) - relOps ) );
      break;
    }
  }
  return *this;
}

bool Atm_condition::eval_one( uint8_t idx ) {
  switch ( _operand[idx].relOp() ) {
    case atm_connector::REL_EQ:
      return _operand[idx].pull( FACTORY ) == _operand[idx].event;
    case atm_connector::REL_NEQ:
      return _operand[idx].pull( FACTORY ) != _operand[idx].event;
    case atm_connector::REL_LT:
      return _operand[idx].pull( FACTORY ) < _operand[idx].event;
    case atm_connector::REL_GT:
      return _operand[idx].pull( FACTORY ) > _operand[idx].event;
    case atm_connector::REL_LTE:
      return _operand[idx].pull( FACTORY ) <= _operand[idx].event;
    case atm_connector::REL_GTE:
      return _operand[idx].pull( FACTORY ) >= _operand[idx].event;
  }
  return false;
}

bool Atm_condition::eval_all() {
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
      }
    }
  }
  return r;
}

int Atm_condition::event( int id ) {
  switch ( id ) {
    case EVT_ON:
      return eval_all();
    case EVT_OFF:
      return !eval_all();
  }
  return 0;
}

void Atm_condition::action( int id ) {
  switch ( id ) {
    case ACT_OFF:
      _connection[_last_state == current ? 3 : 1].push( FACTORY );
      _last_state = current;
      return;
    case ACT_ON:
      if ( _last_state != -1 ) _connection[_last_state == current ? 2 : 0].push( FACTORY );
      _last_state = current;
      return;
  }
}

#ifndef TINYMACHINE
Atm_condition& Atm_condition::trace( Stream& stream ) {
  Machine::setTrace( &stream, atm_serial_debug::trace, "EVT_ON\0EVT_OFF\0EVT_TOGGLE\0EVT_INPUT\0ELSE\0OFF\0ON" );
  return *this;
}
#endif
