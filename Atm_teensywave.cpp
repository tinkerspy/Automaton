#include <Automaton.h>
#include "Atm_teensywave.h"
	
ATM_CLASSNAME & ATM_CLASSNAME::begin( int attached_pin, int steps, int delay )
{
      const static state_t state_table[] PROGMEM = {
      /*                ON_ENTER    ON_LOOP  ON_EXIT  EVT_COUNTER   EVT_TIMER  EVT_TOGGLE   ELSE */
      /* IDLE     */    ACT_IDLE, ATM_SLEEP,      -1,          -1,         -1,         -1,    -1,
      /* START_SN */   ACT_START,        -1,      -1,          -1,         -1,         -1,  SINE,
      /* SINE     */    ACT_SINE,        -1,      -1,    START_SN,       SINE,   START_SW,    -1,
      /* START_SW */   ACT_START,        -1,      -1,          -1,         -1,         -1,   SAW,
      /* SAW      */     ACT_SAW,        -1,      -1,    START_SW,        SAW,   START_SQ,    -1,
      /* START_SQ */ ACT_STARTSQ,        -1,      -1,          -1,         -1,         -1,  SQON,
      /* SQON     */    ACT_SQON,        -1,      -1,          -1,      SQOFF,         -1,    -1,
      /* SQOFF    */   ACT_SQOFF,        -1,      -1,          -1,       SQON,   START_SN,    -1,
	  };
      table( state_table, ELSE );
      pin = attached_pin; 
      _steps = steps;
	  _delay = delay;
      set( timer, delay ); 
      set( phase, steps ); // 314 steps of 0.02 = 6.28 ( 2 * pi )
      _stepsize = (float) 6.28318 / _steps;
      pinMode( pin, OUTPUT ); 
	  analogWriteResolution( 12 );
      return *this;          
}

int ATM_CLASSNAME::event( int id ) 
{
  switch ( id ) {
	case EVT_TIMER :
	  return expired( timer );        
	case EVT_COUNTER :
	  return expired( phase );        
	case EVT_TOGGLE :
	  return signalRead( SIG_TOGGLE );
   }
   return 0;
}

void ATM_CLASSNAME::action( int id ) 
{
  int val;
  switch ( id ) {
	case ACT_IDLE :
	  analogWrite( pin, 0 );
	  return;
	case ACT_START :
      set( timer, _delay ); 
	  set( phase, _steps );
	  return;
	case ACT_STARTSQ :
      set( timer, (int)( _steps * _delay / 2 ) ); 
	  return;
	case ACT_SINE :        
	  val = sin( ( _steps - phase.value ) * _stepsize ) * 2000.0 + 2050.0;
	  analogWrite( pin, val );
	  decrement( phase );
	  return;
	case ACT_SAW :        
	  val = ( _steps - phase.value ) * 13;
	  analogWrite( pin, val );
	  decrement( phase );
	  return;
	case ACT_SQON :        
	  analogWrite( pin, 4095 );
	  return;
	case ACT_SQOFF :        
	  analogWrite( pin, 0 );
	  return;
   }
}