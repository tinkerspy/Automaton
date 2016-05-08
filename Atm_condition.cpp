#include "Atm_condition.hpp"
  
Atm_condition & Atm_condition::begin( bool default_state /* = false */ ) {
  const static state_t state_table[] PROGMEM = {
    /*              ON_ENTER    ON_LOOP  ON_EXIT  EVT_ON  EVT_OFF  EVT_TOGGLE EVT_INPUT ELSE */
    /* OFF     */    ACT_OFF,        -1,      -1,     ON,      -1,         ON,      OFF,  -1,
    /* ON      */     ACT_ON,        -1,      -1,     -1,     OFF,        OFF,       ON,  -1,
  };
  Machine::begin( state_table, ELSE );
  _last_state = -1;
  state( default_state ? ON : OFF );
//  cycle();   Causes the condition to become true momentarily
  return *this;          
}

Atm_condition & Atm_condition::onFlip( bool st, stepcb_t callback )
{
  short idx = st ? 0 : 1;
  _comm[idx]._mode = MODE_CALLBACK;
  _comm[idx]._callback = callback;
  return *this;
}

Atm_condition & Atm_condition::onFlip( bool st, Machine & machine, state_t event /* = 0 */ )
{
  short idx = st ? 0 : 1;
  _comm[idx]._mode = MODE_MACHINE;
  _comm[idx]._client_machine = &machine;
  _comm[idx]._client_machine_event = event;
  return *this;
}

Atm_condition & Atm_condition::onFlip( bool st, const char * label, state_t event /* = 0 */ )
{
  short idx = st ? 0 : 1;
  _comm[idx]._mode = MODE_FACTORY;
  _comm[idx]._client_label = label;
  _comm[idx]._client_label_event = event;
  return *this;
}

Atm_condition & Atm_condition::onFlip( bool st, TinyMachine & machine, state_t event /* = 0 */ )
{
  short idx = st ? 0 : 1;
  _comm[idx]._mode = MODE_TMACHINE;
  _comm[idx]._client_tmachine = &machine;
  _comm[idx]._client_tmachine_event = event;
  return *this;
}

Atm_condition & Atm_condition::onInput( bool st, stepcb_t callback )
{
  short idx = st ? 2 : 3;
  _comm[idx]._mode = MODE_CALLBACK;
  _comm[idx]._callback = callback;
  return *this;
}

Atm_condition & Atm_condition::onInput( bool st, Machine & machine, state_t event /* = 0 */ )
{
  short idx = st ? 2 : 3;
  _comm[idx]._mode = MODE_MACHINE;
  _comm[idx]._client_machine = &machine;
  _comm[idx]._client_machine_event = event;
  return *this;
}

Atm_condition & Atm_condition::onInput( bool st, const char * label, state_t event /* = 0 */ )
{
  short idx = st ? 2 : 3;
  _comm[idx]._mode = MODE_FACTORY;
  _comm[idx]._client_label = label;
  _comm[idx]._client_label_event = event;
  return *this;
}

Atm_condition & Atm_condition::onInput( bool st, TinyMachine & machine, state_t event /* = 0 */ )
{
  short idx = st ? 2 : 3;
  _comm[idx]._mode = MODE_TMACHINE;
  _comm[idx]._client_tmachine = &machine;
  _comm[idx]._client_tmachine_event = event;
  return *this;
}

Atm_condition & Atm_condition::IF( Machine & machine, char relOp /* = '>' */, state_t match /* = 0 */ ) {
  return OP( '?', machine, relOp, match );
}

Atm_condition & Atm_condition::IF( TinyMachine & machine, char relOp /* = '>' */, state_t match /* = 0 */ ) {
  return OP( '?', machine, relOp, match );
}

Atm_condition & Atm_condition::AND( Machine & machine, char relOp /* = '>' */, state_t match /* = 0 */ ) {
  return OP( '&', machine, relOp, match );
}

Atm_condition & Atm_condition::AND( TinyMachine & machine, char relOp /* = '>' */, state_t match /* = 0 */ ) {
  return OP( '&', machine, relOp, match );
}

Atm_condition & Atm_condition::OR( Machine & machine, char relOp /* = '>' */, state_t match /* = 0 */ ) {
  return OP( '|', machine, relOp, match );
}

Atm_condition & Atm_condition::OR( TinyMachine & machine, char relOp /* = '>' */, state_t match /* = 0 */ ) {
  return OP( '|', machine, relOp, match );
}

Atm_condition & Atm_condition::OP( char logOp, Machine & machine, char relOp = '>', state_t match = 0 ) {
  for ( uint8_t i = 0; i <= 4; i++ ) { // Fix me off-by-one, should be <!!!
    if ( _op[i]._mode == MODE_NULL ) { // Pick the first free slot
      _op[i]._mode = MODE_MACHINE;
      _op[i]._logop = logOp;
      _op[i]._relop = relOp;
      _op[i]._client_machine = &machine;
      _op[i]._client_machine_event = match;
      break;
    }
  }
  return *this;
}

Atm_condition & Atm_condition::OP( char logOp, TinyMachine & machine, char relOp, state_t match  ) {
  for ( uint8_t i = 0; i <= 4; i++ ) {
    if ( _op[i]._mode == MODE_NULL ) { // Pick the first free slot
      _op[i]._mode = MODE_TMACHINE;
      _op[i]._logop = logOp;
      _op[i]._relop = relOp;
      _op[i]._client_tmachine = &machine;
      _op[i]._client_tmachine_event = match;
      break;
    }
  }
  return *this;
}

bool Atm_condition::eval_one( uint8_t idx ) {

  if ( _op[idx]._mode == MODE_MACHINE ) {
    state_t machine_state = _op[idx]._client_machine->state();
    switch ( _op[idx]._relop ) {
      case '=' : return machine_state == _op[idx]._client_machine_event;
      case '!' : return machine_state != _op[idx]._client_machine_event;
      case '<' : return machine_state <  _op[idx]._client_machine_event;
      case '>' : return machine_state >  _op[idx]._client_machine_event;
      case '-' : return machine_state <= _op[idx]._client_machine_event;
      case '+' : return machine_state >= _op[idx]._client_machine_event;      
    }       
  }
  if ( _op[idx]._mode == MODE_TMACHINE ) {
    state_t machine_state = _op[idx]._client_tmachine->state();
    switch ( _op[idx]._relop ) {
      case '=' : return machine_state == _op[idx]._client_tmachine_event;
      case '!' : return machine_state != _op[idx]._client_tmachine_event;
      case '<' : return machine_state <  _op[idx]._client_tmachine_event;
      case '>' : return machine_state >  _op[idx]._client_tmachine_event;
      case '-' : return machine_state <= _op[idx]._client_tmachine_event;
      case '+' : return machine_state >= _op[idx]._client_tmachine_event;      
    }       
  }
  return false;
}

bool Atm_condition::eval() {

  bool r = eval_one( 0 );
  for ( uint8_t i = 1; i <= 4; i++ ) {
    if ( _op[i]._mode ) {
      switch ( _op[i]._logop ) {
        case '&' : 
          r = r && eval_one( i );
          break;
        case '|' : 
          r = r || eval_one( i );
          break;
      }
    }   
  }
  return r;
}

int Atm_condition::event( int id ) {
   switch (id ) {
     case EVT_ON :
       return eval();
     case EVT_OFF :
       return !eval();
   }
   return 0;
}

void Atm_condition::comm( Atm_Condition_Comm & c, state_t st ) {

  switch ( c._mode ) {
    case MODE_CALLBACK :
      (*c._callback)( st );
      return;
    case MODE_MACHINE :
      c._client_machine->trigger( c._client_machine_event );
      return;
    case MODE_TMACHINE :
      c._client_tmachine->trigger( c._client_tmachine_event );
      return;
    case MODE_FACTORY:
      factory->trigger( c._client_label, c._client_label_event );
      return;
  }
}

void Atm_condition::action( int id ) {
  switch ( id ) {
    case ACT_OFF :
        comm( _comm[ _last_state == current ? 3 : 1 ], current );
      _last_state = current;
      return;
    case ACT_ON :
      if ( _last_state != -1 )
      comm( _comm[ _last_state == current ? 2 : 0 ], current );
      _last_state = current;
      return;
   }
}

Atm_condition & Atm_condition::trace( Stream & stream ) {
  Machine::setTrace( &stream, atm_serial_debug::trace,
    "EVT_ON\0EVT_OFF\0EVT_TOGGLE\0EVT_INPUT\0ELSE\0OFF\0ON" );
  return *this;
}



