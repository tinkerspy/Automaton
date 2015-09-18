#include <Automaton.h>
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
  set( timer, minimum_duration ); 
  pinMode( pin, INPUT ); 
  return *this;          
}

Atm_pulse & Atm_pulse::onPulse( Machine * machine, uint8_t msg ) 
{
  client_machine = machine;
  client_msg = msg;
  return *this;  
}

Atm_pulse & Atm_pulse::onPulse( pulsecb_t pulse_callback ) 
{
  callback = pulse_callback;
  return *this;  
}

int Atm_pulse::event( int id ) 
{
  switch ( id ) {
  	case EVT_TIMER :
  	  return expired( timer );        
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
      if ( callback ) {
         (*callback)();
         return;
      }
      client_machine->msgWrite( client_msg );
  	  return;
   }
}
