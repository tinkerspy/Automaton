#include <Automaton.h>
#include "Atm_template.h"

Machine & ATM_CLASSNAME::begin( void )
{ 
	const static state_t state_table[] PROGMEM = {
	/*               ON_ENTER  ON_LOOP  ON_EXIT  EVT_TIMER   ELSE */
	/* IDLE   */           -1,      -1,       -1,    STATE1,   -1,
	/* STATE1 */      ACT_ONE,      -1,       -1,    STATE2,   -1,
	/* STATE2 */      ACT_TWO,      -1,       -1,      IDLE,   -1 };
	table( state_table, ELSE );
	set( timer, TIMER );
	return *this;
}

int ATM_CLASSNAME::event( int id ) 
{
  switch ( id ) {
	case EVT_TIMER :
	  return expired( timer );        
  }
  return 0;
}

void ATM_CLASSNAME::action( int id ) 
{
  switch ( id ) {
	case ACT_ONE :
	  // Do something
	  return;
	case ACT_TWO :
	  // Do something else
	  return;
  }
}

