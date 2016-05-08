#include "Atm_multiplier.hpp"
  
Atm_multiplier & Atm_multiplier::begin()
{
  const static state_t state_table[] PROGMEM = {
    /*            ON_ENTER    ON_LOOP  ON_EXIT  EVT_INPUT  ELSE */
    /* IDLE    */       -1, ATM_SLEEP,      -1,      SEND,   -1,
    /* SEND    */ ACT_SEND,        -1,      -1,        -1, IDLE,
  };
  Machine::begin( state_table, ELSE );
  return *this;          
}

Atm_multiplier & Atm_multiplier::onInput( multicb_t callback )
{
  for ( uint8_t idx = 0; idx < _max_comms; idx++ ) {
    if ( _comm[idx]._mode == MODE_NULL ) {
      _comm[idx]._mode = MODE_CALLBACK;
      _comm[idx]._callback = callback;
      break;
    }
  }
  return *this;
}

Atm_multiplier & Atm_multiplier::onInput( Machine & machine, state_t event /* = 0 */ )
{
  for ( uint8_t idx = 0; idx < _max_comms; idx++ ) {
    if ( _comm[idx]._mode == MODE_NULL ) {
      _comm[idx]._mode = MODE_MACHINE;
      _comm[idx]._client_machine = &machine;
      _comm[idx]._client_machine_event = event;
      break;
    }
  }
  return *this;
}

Atm_multiplier & Atm_multiplier::onInput( const char * label, state_t event /* = 0 */ )
{
  for ( uint8_t idx = 0; idx < _max_comms; idx++ ) {
    if ( _comm[idx]._mode == MODE_NULL ) {
      _comm[idx]._mode = MODE_FACTORY;
      _comm[idx]._client_label = label;
      _comm[idx]._client_label_event = event;
      break;
    }
  }
  return *this;
}

Atm_multiplier & Atm_multiplier::onInput( TinyMachine & machine, state_t event /* = 0 */ )
{
  for ( uint8_t idx = 0; idx < _max_comms; idx++ ) {
    if ( _comm[idx]._mode == MODE_NULL ) {    
      _comm[idx]._mode = MODE_TMACHINE;
      _comm[idx]._client_tmachine = &machine;
      _comm[idx]._client_tmachine_event = event;
      break;
    }
  }
  return *this;
}

void Atm_multiplier::comm( Atm_Multiplier_Comm & c ) {

  switch ( c._mode ) {
    case MODE_CALLBACK :
      (*c._callback)();
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

int Atm_multiplier::event( int id ) 
{
   return 0;
}

void Atm_multiplier::action( int id ) 
{
  switch ( id ) {
    case ACT_SEND :
      for ( uint8_t idx = 0; idx < _max_comms; idx++ ) {
        if ( _comm[idx]._mode != MODE_NULL ) {
          comm( _comm[idx] );
        }
      }
      return;
   }
}

Atm_multiplier & Atm_multiplier::trace( Stream & stream ) 
{
  Machine::setTrace( &stream, atm_serial_debug::trace,
    "EVT_INPUT\0ELSE\0IDLE\0SEND" );
  return *this;
}



