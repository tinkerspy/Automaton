#include <Automaton.h>
#include "Atm_scope.h"
	
ATM_CLASSNAME & ATM_CLASSNAME::begin( int attached_pin )
{
  const static state_t state_table[] PROGMEM = {
  /*            ON_ENTER    ON_LOOP  ON_EXIT     ELSE */
  /* LED_ON  */   ACT_ON,        -1,      -1, LED_OFF,
  /* LED_OFF */  ACT_OFF,        -1,      -1,  LED_ON };
  table( state_table, ELSE );
  pin = attached_pin; 
  pinMode( pin, OUTPUT ); 
  return *this;          
}

int ATM_CLASSNAME::event( int id )  
{ 
	return 0;
}

void ATM_CLASSNAME::action( int id ) 
{
  switch ( id ) {
  	case ACT_ON :
  	  digitalWrite( pin, HIGH );
  	  return;
  	case ACT_OFF :
  	  digitalWrite( pin, LOW );
  	  return;
   }
}
