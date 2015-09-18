#include <Automaton.h>
#include "Atm_fade.h"

Atm_fade & Atm_fade::begin( int attached_pin )
{  
	const static state_t state_table[] PROGMEM = {
	/*               ON_ENTER    ON_LOOP       ON_EXIT  EVT_CNT_FADE EVT_TM_FADE   EVT_TM_ON  EVT_TM_OFF   EVT_CNT_RPT    ELSE  */
	/* IDLE   */      ACT_OFF, ATM_SLEEP,           -1,           -1,         -1,         -1,         -1,           -1,     -1,  // LED off
	/* ON     */       ACT_ON, ATM_SLEEP,           -1,           -1,         -1,         -1,         -1,           -1,     -1,  // LED on
	/* START  */      ACT_OFF,        -1,           -1,           -1,         -1,         -1,         -1,           -1, STARTU,  // Start fading
	/* STARTU */    ACT_START,        -1,           -1,           -1,         -1,         -1,         UP,           -1,     -1,  
	/* UP     */       ACT_UP,        -1,           -1,       STARTD,         UP,         -1,         -1,           -1,     -1,
	/* STARTD */    ACT_START,        -1,           -1,           -1,         -1,       DOWN,         -1,           -1,     -1,
	/* DOWN   */     ACT_DOWN,        -1,           -1,       REPEAT,       DOWN,         -1,         -1,           -1,     -1,
	/* REPEAT */   ACT_REPEAT,        -1,           -1,           -1,         -1,         -1,         -1,         IDLE, STARTU };
	Machine::begin( state_table, ELSE );
	pin = attached_pin; 
	pinMode( pin, OUTPUT );
	set( timer_fade, 0 ); // Number of ms per slope step (slope duration: rate * 32 ms)
	set( timer_on, 500 ); // Plateau between slopes (in which led is fully on)
	set( timer_off, 500 ); // Pause between slopes (in which led is fully off)
	set( counter_fade, SLOPE_SIZE );
	set( counter_repeat, ATM_COUNTER_OFF );
	repeat_count = ATM_COUNTER_OFF;
	return *this;
}

Atm_fade & Atm_fade::blink( int duration ) 
{
	set( timer_on, duration ); // Plateau between slopes (in which led is fully on)
	return *this;
}

Atm_fade & Atm_fade::pause( int duration ) 
{
	set( timer_off, duration ); // Pause between slopes (in which led is fully off)
	return *this;
}

Atm_fade & Atm_fade::fade( int fade ) 
{
	set( timer_fade, fade >= 0 ? fade : ATM_TIMER_OFF ); // Number of ms per slope step (slope duration: rate * 32 ms)
	return *this;
}

Atm_fade & Atm_fade::repeat( int repeat ) 
{
	repeat_count = repeat >= 0 ? repeat : ATM_COUNTER_OFF;
    set( counter_repeat, repeat_count );
	return *this;
}

int Atm_fade::event( int id ) 
{
  switch ( id ) {
	case EVT_TM_FADE :
	  return expired( timer_fade );        
	case EVT_TM_ON :
	  return expired( timer_on );        
	case EVT_TM_OFF :
	  return expired( timer_off );        
	case EVT_CNT_FADE :
	  return expired( counter_fade );        
	case EVT_CNT_RPT :
	  return expired( counter_repeat );        
  }
  return 0;
}


void Atm_fade::action( int id ) 
{
  switch ( id ) {
	case ACT_ON :
	  analogWrite( pin, 255 );
	  return;
	case ACT_REPEAT :
	  decrement( counter_repeat );
	  return;
	case ACT_OFF :
	  set( counter_repeat, repeat_count );
	  analogWrite( pin, 0 );
	  return;
	case ACT_START :
	  set( counter_fade, SLOPE_SIZE );
	  return;
	case ACT_UP :
	  analogWrite( pin, slope[SLOPE_SIZE - counter_fade.value] );
	  decrement( counter_fade );
	  return;
	case ACT_DOWN :
	  analogWrite( pin, slope[counter_fade.value - 1] );
	  decrement( counter_fade );
	  return;
  }
}



