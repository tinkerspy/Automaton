#include <Automaton.h>
#include "Atm_blink.h"
	
ATM_CLASSNAME & ATM_CLASSNAME::begin( int attached_pin, int blinkrate )
{
  const static state_t state_table[] PROGMEM = {
  /*            ON_ENTER    ON_LOOP  ON_EXIT  EVT_TIMER   ELSE */
  /* LED_ON  */   ACT_ON,        -1,      -1,   LED_OFF,    -1,
  /* LED_OFF */  ACT_OFF,        -1,      -1,    LED_ON,    -1 };
  table( state_table, ELSE );
  pin = attached_pin; 
  set( timer, blinkrate ); 
  pinMode( pin, OUTPUT ); 
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
  	case ACT_ON :
  	  digitalWrite( pin, HIGH );
  	  return;
  	case ACT_OFF :
  	  digitalWrite( pin, LOW );
  	  return;
   }
}
