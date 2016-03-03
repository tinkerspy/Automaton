#include <Automaton.h>
#include "Atm_fade.h"

Atm_fade & Atm_fade::begin( int attached_pin )
{  
	const static state_t state_table[] PROGMEM = {
	/*               ON_ENTER    ON_LOOP       ON_EXIT  EVT_CNT_FADE EVT_TM_FADE   EVT_TM_ON  EVT_TM_OFF   EVT_CNT_RPT  EVT_ON EVT_OFF EVT_BLINK    ELSE  */
	/* IDLE   */      ACT_OFF, ATM_SLEEP,           -1,           -1,         -1,         -1,         -1,           -1,     ON,     -1,    START,     -1,  // LED off
	/* ON     */       ACT_ON, ATM_SLEEP,           -1,           -1,         -1,         -1,         -1,           -1,     -1,   IDLE,    START,     -1,  // LED on
	/* START  */      ACT_OFF,        -1,           -1,           -1,         -1,         -1,         -1,           -1,     ON,     -1,       -1, STARTU,  // Start fading
	/* STARTU */    ACT_START,        -1,           -1,           -1,         -1,         -1,         UP,           -1,     ON,     -1,       -1,     -1,  
	/* UP     */       ACT_UP,        -1,           -1,       STARTD,         UP,         -1,         -1,           -1,     ON,     -1,       -1,     -1,
	/* STARTD */    ACT_START,        -1,           -1,           -1,         -1,       DOWN,         -1,           -1,     ON,     -1,       -1,     -1,
	/* DOWN   */     ACT_DOWN,        -1,           -1,       REPEAT,       DOWN,         -1,         -1,           -1,     ON,     -1,       -1,     -1,
	/* REPEAT */   ACT_REPEAT,        -1,           -1,           -1,         -1,         -1,         -1,         IDLE,     ON,   IDLE,       -1, STARTU,
    };
	Machine::begin( state_table, ELSE );
    Machine::msgQueue( messages, MSG_END, 1 );
	pin = attached_pin; 
	pinMode( pin, OUTPUT );
	timer_fade.begin( this, 0 ); // Number of ms per slope step (slope duration: rate * 32 ms)
	timer_on.begin( this, 500 ); // Plateau between slopes (in which led is fully on)
	timer_off.begin( this, 500 ); // Pause between slopes (in which led is fully off)
	counter_fade.set( SLOPE_SIZE );
	counter_repeat.set( ATM_COUNTER_OFF );
	repeat_count = ATM_COUNTER_OFF;
	return *this;
}

Atm_fade & Atm_fade::blink( int duration ) 
{
	timer_on.set( duration ); // Plateau between slopes (in which led is fully on)
	return *this;
}

Atm_fade & Atm_fade::pause( int duration ) 
{
	timer_off.set( duration ); // Pause between slopes (in which led is fully off)
	return *this;
}

Atm_fade & Atm_fade::fade( int fade ) 
{
	timer_fade.set( fade >= 0 ? fade : ATM_TIMER_OFF ); // Number of ms per slope step (slope duration: rate * 32 ms)
	return *this;
}

Atm_fade & Atm_fade::repeat( int repeat ) 
{
	repeat_count = repeat >= 0 ? repeat : ATM_COUNTER_OFF;
    counter_repeat.set( repeat_count );
	return *this;
}

int Atm_fade::event( int id ) 
{
  switch ( id ) {
	case EVT_TM_FADE :
	  return timer_fade.expired();        
	case EVT_TM_ON :
	  return timer_on.expired();        
	case EVT_TM_OFF :
	  return timer_off.expired();        
	case EVT_CNT_FADE :
	  return counter_fade.expired();        
	case EVT_CNT_RPT :
	  return counter_repeat.expired();      
    case EVT_ON :
      return msgRead( MSG_ON );    
    case EVT_OFF :
      return msgRead( MSG_OFF );    
    case EVT_BLINK :
      return msgRead( MSG_BLINK );    
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
	  counter_repeat.decrement();
	  return;
	case ACT_OFF :
	  counter_repeat.set( repeat_count );
	  analogWrite( pin, 0 );
	  return;
	case ACT_START :
	  counter_fade.set( SLOPE_SIZE );
	  return;
	case ACT_UP :
	  analogWrite( pin, slope[SLOPE_SIZE - counter_fade.value] );
	  counter_fade.decrement();
	  return;
	case ACT_DOWN :
	  analogWrite( pin, slope[counter_fade.value - 1] );
	  counter_fade.decrement();
	  return;
  }
}



