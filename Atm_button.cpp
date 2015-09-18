#include <Automaton.h>
#include "Atm_button.h"

// Add option for button press callback (for reading i2c buttons etc)

Atm_button & Atm_button::begin( int attached_pin, presscb_t press_callback )
{
	const static state_t state_table[] PROGMEM = {
	/* Standard Mode: press/repeat */
		/*                  ON_ENTER  ON_LOOP       ON_EXIT  EVT_LMODE  EVT_TIMER  EVT_DELAY  EVT_REPEAT EVT_PRESS  EVT_RELEASE  EVT_COUNTER   EVT_AUTO  ELSE */
		/* IDLE     */            -1,      -1,           -1,     LIDLE,        -1,        -1,         -1,     WAIT,          -1,          -1,      AUTO,   -1,
		/* WAIT     */            -1,      -1,           -1,        -1,   PRESSED,        -1,         -1,       -1,        IDLE,          -1,        -1,    -1,
		/* PRESSED  */     ACT_PRESS,      -1,           -1,        -1,        -1,    REPEAT,         -1,       -1,     RELEASE,          -1,        -1,    -1,
		/* REPEAT   */     ACT_PRESS,      -1,           -1,        -1,        -1,        -1,     REPEAT,       -1,     RELEASE,          -1,        -1,    -1,
		/* RELEASE  */   ACT_RELEASE,      -1,           -1,        -1,        -1,        -1,         -1,       -1,          -1,          -1,        -1,  IDLE,
	/* Long Press Mode: press/long press */	
		/* LIDLE    */            -1,      -1,           -1,        -1,        -1,        -1,         -1,    LWAIT,          -1,          -1,        -1,    -1,
		/* LWAIT    */    ACT_LSTART,      -1,           -1,        -1,  LPRESSED,        -1,         -1,       -1,       LIDLE,          -1,        -1,    -1,
		/* LPRESSED */    ACT_LCOUNT,      -1,           -1,        -1,        -1,  LPRESSED,         -1,       -1,    LRELEASE,    WRELEASE,        -1,    -1,
		/* LRELEASE */  ACT_LRELEASE,      -1, ACT_WRELEASE,        -1,        -1,        -1,         -1,       -1,          -1,          -1,        -1, LIDLE,
		/* WRELEASE */  ACT_LRELEASE,      -1, ACT_WRELEASE,        -1,        -1,        -1,         -1,       -1,       LIDLE,          -1,        -1,    -1,
		/* AUTO     */      ACT_AUTO,      -1,           -1,        -1,        -1,        -1,         -1,       -1,          -1,          -1,        -1,  IDLE,
	};
	Machine::begin( state_table, ELSE );
	pin = attached_pin;
    set( counter_longpress, 0 );	
	callback = press_callback;
	set( timer_debounce, DEBOUNCE );
	set( timer_delay, ATM_TIMER_OFF );
	set( timer_repeat, ATM_TIMER_OFF );
	set( timer_auto, ATM_TIMER_OFF );
	pinMode( pin, INPUT_PULLUP );
	return *this;
}

Atm_button & Atm_button::debounce( int delay ) {
	
	set( timer_debounce, delay );
	return *this;
}

Atm_button & Atm_button::longPress( int max, int delay ) {
	
	longpress_max = max;
    set( counter_longpress, longpress_max );
	set( timer_delay, delay );
	return *this;
}

Atm_button & Atm_button::repeat( int delay, int speed ) {
	
	set( timer_delay, delay );
	set( timer_repeat, speed );	
	return *this;
}

Atm_button & Atm_button::repeat( void ) {
	
	set( timer_delay, 500 );
	set( timer_repeat, 50 );	
	return *this;
}

Atm_button & Atm_button::autoPress( int delay, int press ) {
	
    _auto_press = press;
	set( timer_auto, delay );    
	return *this;
}

Atm_button & Atm_button::loHi( void ) {
	
    lohi = 1;
	return *this;
}

int Atm_button::event( int id ) 
{
  switch ( id ) {
	case EVT_LMODE :
	  return counter_longpress.value > 0;        
	case EVT_TIMER :
	  return expired( timer_debounce );        
	case EVT_DELAY :
	  return expired( timer_delay );        
	case EVT_REPEAT :
	  return expired( timer_repeat );        
	case EVT_AUTO :
	  return expired( timer_auto );        
	case EVT_PRESS :
	  return pinChange( pin, LOW );        
	case EVT_RELEASE :
	  return pinChange( pin, HIGH );        
	case EVT_COUNTER :
	  return expired( counter_longpress );        
  }
  return 0;
}

void Atm_button::action( int id ) 
{
  switch ( id ) {
	case ACT_PRESS :
	  (*callback)( 1 );
	  return;
	case ACT_AUTO :
	  (*callback)( _auto_press );
	  return;
	case ACT_RELEASE :
	  (*callback)( 0 );
	  return;
	case ACT_LSTART :
	  set( counter_longpress, longpress_max );
	  return;
	case ACT_LCOUNT :
	  decrement( counter_longpress );
	  (*callback)( ( longpress_max - counter_longpress.value ) * -1 );	  
	  return;
	case ACT_LRELEASE :
	  (*callback)( longpress_max - counter_longpress.value );
	  return;
	case ACT_WRELEASE :
	  (*callback)( 0 );
	  return;
  }
}
