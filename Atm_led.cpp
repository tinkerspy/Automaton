#include "Atm_led.h"

Atm_led & Atm_led::begin( int attached_pin )
{ 
	static const state_t state_table[] PROGMEM = {
	/*               ON_ENTER    ON_LOOP    ON_EXIT  EVT_ON_TIMER  EVT_OFF_TIMER  EVT_COUNTER  EVT_ON  EVT_OFF  EVT_BLINK  EVT_TOGGLE  EVT_TOGGLE_BLINK ELSE */
	/* IDLE      */  ACT_INIT, ATM_SLEEP,        -1,           -1,            -1,          -1,     ON,      -1,     START,         ON,            START,  -1, // LED off
	/* ON        */    ACT_ON, ATM_SLEEP,        -1,           -1,            -1,          -1,     -1,    IDLE,     START,       IDLE,             IDLE,  -1, // LED on
	/* START     */    ACT_ON,        -1,        -1,    BLINK_OFF,            -1,        IDLE,     ON,    IDLE,     START,       IDLE,             IDLE,  -1, // Start blinking
	/* BLINK_OFF */   ACT_OFF,        -1,        -1,           -1,         START,        DONE,     ON,    IDLE,     START,       IDLE,             IDLE,  -1,
	/* DONE      */        -1,        -1, ACT_CHAIN,           -1,          IDLE,          -1,     ON,    IDLE,     START,       IDLE,             IDLE,  -1, // Wait after last blink
    };
	Machine::begin( state_table, ELSE );
	pin = attached_pin; 
	pinMode( pin, OUTPUT );
    on_timer.set( 500 );    
    off_timer.set( 500 );    
	repeat_count = ATM_COUNTER_OFF;
	counter.set( repeat_count );
	return *this;
}

Atm_led & Atm_led::chain( Machine & n, uint8_t event /* = EVT_BLINK */ ) 
{
    chain_next = &n;
    chain_previous = &n;    
    chain_event = event;
    flags &= ~ATM_USR1_FLAG;
    return *this;
}

Atm_led & Atm_led::chain( Machine & n, Machine & p, uint8_t event /* = EVT_BLINK */) 
{
    chain_next = &n;
    chain_previous = &p;    
    chain_event = event;
    flags &= ~ATM_USR1_FLAG;
    return *this;
}

Atm_led & Atm_led::blink( uint32_t duration, uint32_t pause_duration, uint16_t repeat_count /* = ATM_COUNTER_OFF */ ) {
	
	blink( duration ); // Time in which led is fully on
    pause( pause_duration );
    repeat( repeat_count );
}

Atm_led & Atm_led::blink( uint32_t duration ) 
{
	on_timer.set( duration ); // Time in which led is fully on
	return *this;
}

Atm_led & Atm_led::pause( uint32_t duration ) 
{
	off_timer.set( duration ); // Time in which led is fully off
	return *this;
}

Atm_led & Atm_led::fade( int fade ) { return *this; } // Dummy for method compatibility with Atm_fade

Atm_led & Atm_led::repeat( int repeat ) 
{
	repeat_count = repeat >= 0 ? repeat : ATM_COUNTER_OFF;
    counter.set( repeat_count );
	return *this;
}

int Atm_led::event( int id ) 
{
	switch ( id ) {
		case EVT_ON_TIMER :
			return on_timer.expired( this );        
		case EVT_OFF_TIMER :
			return off_timer.expired( this );        
		case EVT_COUNTER :
			return counter.expired();
	}
	return 0;
}

void Atm_led::action( int id ) 
{
	switch ( id ) {
		case ACT_INIT :
			digitalWrite( pin, LOW );
			counter.set( repeat_count );
			return;
		case ACT_ON :
			digitalWrite( pin, HIGH );
			return;
		case ACT_OFF :
			counter.decrement();
			digitalWrite( pin, LOW );
			return;
        case ACT_CHAIN :            
            if ( chain_next ) {
              if ( ( flags & ATM_USR1_FLAG ) > 0 ) {
                chain_previous->trigger( chain_event );
              } else {
                chain_next->trigger( chain_event );
              }              
              flags ^= ATM_USR1_FLAG;
            }  
            return;
	}
}

Atm_led & Atm_led::trace( Stream & stream ) {

  setTrace( &stream, atm_serial_debug::trace, 
    "EVT_ON_TIMER\0EVT_OFF_TIMER\0EVT_COUNTER\0EVT_ON\0EVT_OFF\0EVT_BLINK\0EVT_TOGGLE\0EVT_TOGGLE_BLINK\0ELSE\0IDLE\0ON\0START\0BLINK_OFF\0DONE" );
  return *this;
}


// TinyMachine version

Att_led & Att_led::begin( int attached_pin )
{ 
	static const tiny_state_t state_table[] PROGMEM = {
	/*               ON_ENTER    ON_LOOP    ON_EXIT  EVT_ON_TIMER  EVT_OFF_TIMER  EVT_COUNTER  EVT_ON  EVT_OFF  EVT_BLINK  ELSE */
	/* IDLE      */  ACT_INIT, ATM_SLEEP,        -1,           -1,            -1,          -1,     ON,      -1,     START,   -1, // LED off
	/* ON        */    ACT_ON, ATM_SLEEP,        -1,           -1,            -1,          -1,     -1,    IDLE,     START,   -1, // LED on
	/* START     */    ACT_ON,        -1,        -1,    BLINK_OFF,            -1,        IDLE,     ON,    IDLE,     START,   -1, // Start blinking
	/* BLINK_OFF */   ACT_OFF,        -1,        -1,           -1,         START,        DONE,     ON,    IDLE,     START,   -1,
	/* DONE      */        -1,        -1, ACT_CHAIN,           -1,          IDLE,          -1,     ON,    IDLE,     START,   -1, // Wait after last blink
    };
	TinyMachine::begin( state_table, ELSE );
	pin = attached_pin; 
	pinMode( pin, OUTPUT );
    on_timer.set( 500 );    
    off_timer.set( 500 );    
	repeat_count = ATM_COUNTER_OFF;
	counter.set( repeat_count );
	return *this;
}


Att_led & Att_led::chain( TinyMachine & n, uint8_t event /* = EVT_BLINK */ ) 
{
    chain_next = &n;
    chain_previous = &n;    
    chain_event = event;
    flags &= ~ATM_USR1_FLAG;
    return *this;
}

Att_led & Att_led::chain( TinyMachine & n, TinyMachine & p, uint8_t event /* = EVT_BLINK */) 
{
    chain_next = &n;
    chain_previous = &p;    
    chain_event = event;
    flags &= ~ATM_USR1_FLAG;
    return *this;
}

Att_led & Att_led::blink( uint32_t duration ) 
{
	on_timer.set( duration ); // Time in which led is fully on
	return *this;
}

Att_led & Att_led::pause( uint32_t duration ) 
{
	off_timer.set( duration ); // Time in which led is fully off
	return *this;
}

Att_led & Att_led::fade( int fade ) { return *this; } // Dummy for method compatibility with Att_fade

Att_led & Att_led::repeat( int repeat ) 
{
	repeat_count = repeat >= 0 ? repeat : ATM_COUNTER_OFF;
    counter.set( repeat_count );
	return *this;
}

int Att_led::event( int id ) 
{
	switch ( id ) {
		case EVT_ON_TIMER :
			return on_timer.expired( this );        
		case EVT_OFF_TIMER :
			return off_timer.expired( this );        
		case EVT_COUNTER :
			return counter.expired();
	}
	return 0;
}

void Att_led::action( int id ) 
{
	switch ( id ) {
		case ACT_INIT :
			digitalWrite( pin, LOW );
			counter.set( repeat_count );
			return;
		case ACT_ON :
			digitalWrite( pin, HIGH );
			return;
		case ACT_OFF :
			counter.decrement();
			digitalWrite( pin, LOW );
			return;
        case ACT_CHAIN :            
            if ( chain_next ) {
              if ( ( flags & ATM_USR1_FLAG ) > 0 ) {
                chain_previous->trigger( chain_event );
              } else {
                chain_next->trigger( chain_event );
              }              
              flags ^= ATM_USR1_FLAG;
            }  
            return;
	}
}
