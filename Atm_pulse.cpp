#include "Atm_pulse.hpp"
	
Atm_pulse & Atm_pulse::begin( int attached_pin, int minimum_duration /* = 20 */, bool activeLow /* = false */, bool pullUp /* = false */ )
{
  const static state_t state_table[] PROGMEM = {
  /*              ON_ENTER    ON_LOOP      ON_EXIT  EVT_TIMER   EVT_HIGH  EVT_LOW   ELSE */
  /* IDLE    */         -1,        -1,          -1,        -1,      WAIT,      -1,    -1,
  /* WAIT    */         -1,        -1,          -1,     PULSE,        -1,    IDLE,    -1,
  /* PULSE   */  ACT_PULSE,        -1, ACT_RELEASE,        -1,        -1,    IDLE,    -1,
  };
  Machine::begin( state_table, ELSE );
  pin = attached_pin; 
  _activeLow = activeLow;
  timer.set( minimum_duration );
  pinMode( pin, pullUp ? INPUT_PULLUP : INPUT ); 
  return *this;          
}

Atm_pulse & Atm_pulse::onPulse( pulsecb_t callback, int idx /* = 0 */ )
{
  _onpulse._mode = MODE_CALLBACK;  
  _onpulse._callback = callback;
  _onpulse._callback_idx = idx;
  _onpulse._callback_count = 0;
  return *this;
}

Atm_pulse & Atm_pulse::onPulse( Machine & machine, int event /* = 0 */ )
{
  _onpulse._mode = MODE_MACHINE;  
  _onpulse._client_machine = &machine;
  _onpulse._client_machine_event = event;
  return *this;
}

Atm_pulse & Atm_pulse::onPulse( const char * label, int event /* = 0 */ )
{
  _onpulse._mode = MODE_FACTORY;  
  _onpulse._client_label = label;
  _onpulse._client_label_event = event;
  return *this;
}

Atm_pulse & Atm_pulse::onPulse( TinyMachine & machine, int event /* = 0 */ )
{
  _onpulse._mode = MODE_TMACHINE;    
  _onpulse._client_tmachine = &machine;
  _onpulse._client_tmachine_event = event;
  return *this;
}

Atm_pulse & Atm_pulse::onRelease( pulsecb_t callback, int idx /* = 0 */ )
{
  _onrelease._mode = MODE_CALLBACK;  
  _onrelease._callback = callback;
  _onrelease._callback_idx = idx;
  _onrelease._callback_count = 0;
  return *this;
}

Atm_pulse & Atm_pulse::onRelease( Machine & machine, int event /* = 0 */ )
{
  _onrelease._mode = MODE_MACHINE;
  _onrelease._client_machine = &machine;
  _onrelease._client_machine_event = event;
  return *this;
}

Atm_pulse & Atm_pulse::onRelease( const char * label, int event /* = 0 */ )
{
  _onrelease._mode = MODE_FACTORY;
  _onrelease._client_label = label;
  _onrelease._client_label_event = event;
  return *this;
}

Atm_pulse & Atm_pulse::onRelease( TinyMachine & machine, int event /* = 0 */ )
{
  _onrelease._mode = MODE_TMACHINE;
  _onrelease._client_tmachine = &machine;
  _onrelease._client_tmachine_event = event;
  return *this;
}

int Atm_pulse::event( int id ) 
{
  switch ( id ) {
  	case EVT_TIMER :
  	  return timer.expired( this );
    case EVT_HIGH :
      return ( !digitalRead( pin ) != !_activeLow ); // XOR
    case EVT_LOW :
      return !( !digitalRead( pin ) != !_activeLow );
   }
   return 0;
}

void Atm_pulse::action( int id ) 
{
  switch ( id ) {
  	case ACT_PULSE :
      if ( _onpulse._mode == MODE_CALLBACK ) {
        _onpulse._callback_count++;
        (*_onpulse._callback)( _onpulse._callback_idx, _onpulse._callback_count );
      }
      if ( _onpulse._mode == MODE_MACHINE ) {
        _onpulse._client_machine->trigger( _onpulse._client_machine_event );
      }
      if ( _onpulse._mode == MODE_FACTORY && factory ) {
        factory->trigger( _onpulse._client_label, _onpulse._client_label_event );
      }
      if ( _onpulse._mode == MODE_TMACHINE ) {
        _onpulse._client_tmachine->trigger( _onpulse._client_tmachine_event );
      }
  	  return;
  	case ACT_RELEASE :
      if ( _onrelease._mode == MODE_CALLBACK ) {
        _onrelease._callback_count++;
        (*_onrelease._callback)( _onrelease._callback_idx, _onrelease._callback_count );
      }
      if ( _onrelease._mode == MODE_MACHINE ) {
        _onrelease._client_machine->trigger( _onrelease._client_machine_event );
      }
      if ( _onrelease._mode == MODE_FACTORY && factory ) {
        factory->trigger( _onrelease._client_label, _onrelease._client_label_event );
      }
      if ( _onrelease._mode == MODE_TMACHINE ) {
        _onrelease._client_tmachine->trigger( _onrelease._client_tmachine_event );
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


	
Att_pulse & Att_pulse::begin( int attached_pin, int minimum_duration /* = 20 */, bool activeLow /* = false */, bool pullUp /* = false */ )
{
  const static tiny_state_t state_table[] PROGMEM = {
  /*              ON_ENTER    ON_LOOP      ON_EXIT  EVT_TIMER   EVT_HIGH  EVT_LOW   ELSE */
  /* IDLE    */         -1,        -1,          -1,        -1,      WAIT,      -1,    -1,
  /* WAIT    */         -1,        -1,          -1,     PULSE,        -1,    IDLE,    -1,
  /* PULSE   */  ACT_PULSE,        -1, ACT_RELEASE,        -1,        -1,    IDLE,    -1,
  };
  TinyMachine::begin( state_table, ELSE );
  pin = attached_pin; 
  _activeLow = activeLow;
  timer.set( minimum_duration );
  pinMode( pin, pullUp ? INPUT_PULLUP : INPUT ); 
  return *this;          
}

Att_pulse & Att_pulse::onPulse( pulsecb_t callback, int idx /* = 0 */ )
{
  _onpulse._mode = MODE_CALLBACK;  
  _onpulse._callback = callback;
  _onpulse._callback_idx = idx;
  _onpulse._callback_count = 0;
  return *this;
}

Att_pulse & Att_pulse::onPulse( Machine & machine, int event /* = 0 */ )
{
  _onpulse._mode = MODE_MACHINE;  
  _onpulse._client_machine = &machine;
  _onpulse._client_machine_event = event;
  return *this;
}

Att_pulse & Att_pulse::onPulse( TinyMachine & machine, int event /* = 0 */ )
{
  _onpulse._mode = MODE_TMACHINE;    
  _onpulse._client_tmachine = &machine;
  _onpulse._client_tmachine_event = event;
  return *this;
}

Att_pulse & Att_pulse::onRelease( pulsecb_t callback, int idx /* = 0 */ )
{
  _onrelease._mode = MODE_CALLBACK;  
  _onrelease._callback = callback;
  _onrelease._callback_idx = idx;
  _onrelease._callback_count = 0;
  return *this;
}

Att_pulse & Att_pulse::onRelease( Machine & machine, int event /* = 0 */ )
{
  _onrelease._mode = MODE_MACHINE;
  _onrelease._client_machine = &machine;
  _onrelease._client_machine_event = event;
  return *this;
}

Att_pulse & Att_pulse::onRelease( TinyMachine & machine, int event /* = 0 */ )
{
  _onrelease._mode = MODE_TMACHINE;
  _onrelease._client_tmachine = &machine;
  _onrelease._client_tmachine_event = event;
  return *this;
}

int Att_pulse::event( int id ) 
{
  switch ( id ) {
  	case EVT_TIMER :
  	  return timer.expired( this );
    case EVT_HIGH :
      return ( !digitalRead( pin ) != !_activeLow ); // XOR
    case EVT_LOW :
      return !( !digitalRead( pin ) != !_activeLow );
   }
   return 0;
}

void Att_pulse::action( int id ) 
{
  switch ( id ) {
  	case ACT_PULSE :
      if ( _onpulse._mode == MODE_CALLBACK ) {
        _onpulse._callback_count++;
        (*_onpulse._callback)( _onpulse._callback_idx, _onpulse._callback_count );
      }
      if ( _onpulse._mode == MODE_MACHINE ) {
        _onpulse._client_machine->trigger( _onpulse._client_machine_event );
      }
      if ( _onpulse._mode == MODE_TMACHINE ) {
        _onpulse._client_tmachine->trigger( _onpulse._client_tmachine_event );
      }
  	  return;
  	case ACT_RELEASE :
      if ( _onrelease._mode == MODE_CALLBACK ) {
        _onrelease._callback_count++;
        (*_onrelease._callback)( _onrelease._callback_idx, _onrelease._callback_count );
      }
      if ( _onrelease._mode == MODE_MACHINE ) {
        _onrelease._client_machine->trigger( _onrelease._client_machine_event );
      }
      if ( _onrelease._mode == MODE_TMACHINE ) {
        _onrelease._client_tmachine->trigger( _onrelease._client_tmachine_event );
      }
  	  return;
   }
}
