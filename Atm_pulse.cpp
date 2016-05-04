#include "Atm_pulse.h"
	
Atm_pulse & Atm_pulse::begin( int attached_pin, int minimum_duration )
{
  const static state_t state_table[] PROGMEM = {
  /*              ON_ENTER    ON_LOOP  ON_EXIT  EVT_TIMER   EVT_HIGH  EVT_LOW   ELSE */
  /* IDLE    */         -1,        -1,      -1,        -1,      WAIT,      -1,    -1,
  /* WAIT    */         -1,        -1,      -1,     PULSE,        -1,    IDLE,    -1,
  /* PULSE   */  ACT_PULSE,        -1,      -1,        -1,        -1,    IDLE,    -1,
  };
  Machine::begin( state_table, ELSE );
  pin = attached_pin; 
  timer.set( minimum_duration );
  pinMode( pin, INPUT ); 
  return *this;          
}

Atm_pulse & Atm_pulse::onPulse( pulsecb_t callback, int idx /* = 0 */ )
{
  _callback = callback;
  _callback_idx = idx;
  _callback_count = 0;
  flags &= ~ATM_USR_FLAGS;
  flags |= ATM_USR1_FLAG;
  return *this;
}

Atm_pulse & Atm_pulse::onPulse( Machine & machine, int event /* = 0 */ )
{
  _client_machine = &machine;
  _client_machine_event = event;
  flags &= ~ATM_USR_FLAGS;
  flags |= ATM_USR2_FLAG;
  return *this;
}

Atm_pulse & Atm_pulse::onPulse( const char * label, int event /* = 0 */ )
{
  _client_label = label;
  _client_label_event = event;
  flags &= ~ATM_USR_FLAGS;
  flags |= ATM_USR3_FLAG;
  return *this;
}

Atm_pulse & Atm_pulse::onPulse( TinyMachine & machine, int event /* = 0 */ )
{
  _client_tmachine = &machine;
  _client_tmachine_event = event;
  flags &= ~ATM_USR_FLAGS;
  flags |= ATM_USR4_FLAG;
  return *this;
}

int Atm_pulse::event( int id ) 
{
  switch ( id ) {
  	case EVT_TIMER :
  	  return timer.expired( this );
    case EVT_HIGH :
      return digitalRead( pin );
    case EVT_LOW :
      return !digitalRead( pin );
   }
   return 0;
}

void Atm_pulse::action( int id ) 
{
  switch ( id ) {
  	case ACT_PULSE :
      if ( ( flags & ATM_USR1_FLAG ) > 0 ) {
        _callback_count++;
        (*_callback)( _callback_idx, _callback_count );
      }
      if ( ( flags & ATM_USR2_FLAG ) > 0 ) {
        _client_machine->trigger( _client_machine_event );
      }
      if ( ( flags & ATM_USR3_FLAG ) > 0 && factory ) {
        factory->trigger( _client_label, _client_label_event );
      }
      if ( ( flags & ATM_USR4_FLAG ) > 0 ) {
        _client_tmachine->trigger( _client_tmachine_event );
      }
  	  return;
   }
}

Atm_pulse & Atm_pulse::trace( Stream & stream ) {

  setTrace( &stream, atm_serial_debug::trace,
    "EVT_TIMER\0EVT_HIGH\0EVT_LOW\0ELSE\0IDLE\0WAIT\0PULSE" );
  return *this;
}


// TinyMachine version

Att_pulse & Att_pulse::begin( int attached_pin, int minimum_duration )
{
  const static tiny_state_t state_table[] PROGMEM = {
  /*              ON_ENTER    ON_LOOP  ON_EXIT  EVT_TIMER   EVT_HIGH  EVT_LOW   ELSE */
  /* IDLE    */         -1,        -1,      -1,        -1,      WAIT,      -1,    -1,
  /* WAIT    */         -1,        -1,      -1,     PULSE,        -1,    IDLE,    -1,
  /* PULSE   */  ACT_PULSE,        -1,      -1,        -1,        -1,    IDLE,    -1,
  };
  TinyMachine::begin( state_table, ELSE );
  pin = attached_pin; 
  timer.set( minimum_duration );
  pinMode( pin, INPUT );
  return *this;          
}

Att_pulse & Att_pulse::onPulse( pulsecb_t callback, int idx /* = 0 */ )
{
  _callback = callback;
  _callback_idx = idx;
  _callback_count = 0;
  flags &= ~ATM_USR_FLAGS;
  flags |= ATM_USR1_FLAG;
  return *this;
}

Att_pulse & Att_pulse::onPulse( Machine & machine, int event /* = 0 */ )
{
  _client_machine = &machine;
  _client_machine_event = event;
  flags &= ~ATM_USR_FLAGS;
  flags |= ATM_USR2_FLAG;
  return *this;
}

Att_pulse & Att_pulse::onPulse( TinyMachine & machine, int event /* = 0 */ )
{
  _client_tmachine = &machine;
  _client_tmachine_event = event;
  flags &= ~ATM_USR_FLAGS;
  flags |= ATM_USR4_FLAG;
  return *this;
}

int Att_pulse::event( int id ) 
{
  switch ( id ) {
  	case EVT_TIMER :
  	  return timer.expired( this );
    case EVT_HIGH :
      return digitalRead( pin );
    case EVT_LOW :
      return !digitalRead( pin );
   }
   return 0;
}

void Att_pulse::action( int id ) 
{
  switch ( id ) {
    case ACT_PULSE :
      if ( ( flags & ATM_USR1_FLAG ) > 0 ) {
        _callback_count++;
        (*_callback)( _callback_idx, _callback_count );
      }
      if ( ( flags & ATM_USR2_FLAG ) > 0 ) {
        _client_machine->trigger( _client_machine_event );
      }
      if ( ( flags & ATM_USR4_FLAG ) > 0 ) {
        _client_tmachine->trigger( _client_tmachine_event );
      }
  	  return;
   }
}
