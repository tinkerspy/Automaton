#include <Automaton.h>
#include "Atm_blink.h"
	
Atm_blink & Atm_blink::begin( int attached_pin, uint32_t blinkrate )
{
  const static state_t state_table[] PROGMEM = {
  /*            ON_ENTER    ON_LOOP  ON_EXIT  EVT_TIMER   ELSE */
  /* LED_ON  */   ACT_ON,        -1,      -1,   LED_OFF,    -1,
  /* LED_OFF */  ACT_OFF,        -1,      -1,    LED_ON,    -1 };
  Machine::begin( state_table, ELSE );
  pin = attached_pin; 
  timer.begin( this, blinkrate ); 
  pinMode( pin, OUTPUT ); 
  return *this;          
}

int Atm_blink::event( int id ) 
{
  switch ( id ) {
  	case EVT_TIMER :
  	  return timer.expired();        
   }
   return 0;
}

void Atm_blink::action( int id ) 
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

Atm_blink & Atm_blink::onSwitch( swcb_sym_t switch_callback ) {

  Machine::onSwitch( switch_callback, "LED_ON\0LED_OFF", "EVT_TIMER\0ELSE" );
  return *this;
}

