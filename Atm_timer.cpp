#include <Automaton.h>
#include "Atm_timer.h"
	
Atm_timer & Atm_timer::begin( void )
{
  const static state_t state_table[] PROGMEM = {
  /*             ON_ENTER    ON_LOOP  ON_EXIT  EVT_TIMER   EVT_COUNTER  EVT_OFF  EVT_ON  ELSE */
  /* IDLE    */        -1, ATM_SLEEP,      -1,        -1,           -1,      -1,   WAIT,   -1,
  /* WAIT    */        -1,        -1,      -1,   TRIGGER,           -1,    IDLE,     -1,   -1,
  /* TRIGGER */  ACT_TRIG,        -1,      -1,        -1,         IDLE,    IDLE,     -1, WAIT,
  };
  Machine::begin( state_table, ELSE );
  Machine::msgQueue( messages, MSG_END, 1 );
  timer.begin( this, ATM_TIMER_OFF ); 
  counter.set( 1 ); 
  state( WAIT );
  return *this;          
}


Atm_timer & Atm_timer::onTimer( Machine * machine, uint8_t msg ) 
{
  client_machine = machine;
  client_msg = msg;
  return *this;  
}

Atm_timer & Atm_timer::onTimer( timer_cb_t timer_callback ) 
{
  callback = timer_callback;
  return *this;  
}

Atm_timer & Atm_timer::interval( int v )
{
  timer.set( v );  
  state( WAIT );
  return *this;
}

Atm_timer & Atm_timer::repeat( int v )
{
  counter.set( v );  
  state( WAIT );
  return *this;
}

Atm_timer & Atm_timer::id( int v )
{
  timer_id = v;  
  return *this;
}

int Atm_timer::event( int id ) 
{
  switch ( id ) {
    case EVT_COUNTER :
      return counter.expired();        
    case EVT_TIMER :
      return timer.expired();        
    case EVT_OFF :
      return msgRead( MSG_OFF );        
    case EVT_ON :
      return msgRead( MSG_ON );        
   }
   return 0;
}

void Atm_timer::action( int id ) 
{
  switch ( id ) {
  	case ACT_TRIG :
      counter.decrement();
      if ( callback ) {
         (*callback)( timer_id );
      }
      if ( client_machine ) {
        client_machine->msgWrite( client_msg );
      }
      return;
   }
}

Atm_timer & Atm_timer::onSwitch( swcb_sym_t switch_callback ) {

  Machine::onSwitch( switch_callback, 
    "IDLE\0WAIT\0TRIGGER", 
    "EVT_TIMER\0EVT_COUNTER\0EVT_OFF\0EVT_ON\0ELSE" );  
  return *this;
}

// TinyMachine version
	
Att_timer & Att_timer::begin( void )
{
  const static tiny_state_t state_table[] PROGMEM = {
  /*             ON_ENTER    ON_LOOP  ON_EXIT  EVT_TIMER   EVT_COUNTER  EVT_OFF  EVT_ON  ELSE */
  /* IDLE    */        -1, ATM_SLEEP,      -1,        -1,           -1,      -1,   WAIT,   -1,
  /* WAIT    */        -1,        -1,      -1,   TRIGGER,           -1,    IDLE,     -1,   -1,
  /* TRIGGER */  ACT_TRIG,        -1,      -1,        -1,         IDLE,    IDLE,     -1, WAIT,
  };
  TinyMachine::begin( state_table, ELSE );
  timer.begin( this, ATM_TIMER_OFF ); 
  counter.set( 1 ); 
  state( WAIT );
  return *this;          
}


Att_timer & Att_timer::onTimer( Machine * machine, uint8_t msg ) 
{
  client_machine = machine;
  client_msg = msg;
  return *this;  
}

Att_timer & Att_timer::onTimer( timer_cb_t timer_callback ) 
{
  callback = timer_callback;
  return *this;  
}

Att_timer & Att_timer::interval( int v )
{
  timer.set( v );  
  state( WAIT );
  return *this;
}

Att_timer & Att_timer::repeat( int v )
{
  counter.set( v );  
  state( WAIT );
  return *this;
}

Att_timer & Att_timer::id( int v )
{
  timer_id = v;  
  return *this;
}

int Att_timer::event( int id ) 
{
  switch ( id ) {
    case EVT_COUNTER :
      return counter.expired();        
    case EVT_TIMER :
      return timer.expired();        
   }
   return 0;
}

void Att_timer::action( int id ) 
{
  switch ( id ) {
  	case ACT_TRIG :
      counter.decrement();
      if ( callback ) {
         (*callback)( timer_id );
      }
      if ( client_machine ) {
        client_machine->msgWrite( client_msg );
      }
      return;
   }
}


