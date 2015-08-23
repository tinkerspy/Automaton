
#ifndef Atm_led_h
#define Atm_led_h

#include <Automaton.h>

#undef ATM_CLASSNAME
#define ATM_CLASSNAME Atm_led

class ATM_CLASSNAME : public Machine {

  public:
  	// Class constructor
	ATM_CLASSNAME( void ) : Machine() { class_label = "LED"; };

	// Custom state, event, action & signal constants (enum)
	enum { IDLE, ON, START, BLINK_OFF } STATES; 
	enum { EVT_ON_TIMER, EVT_OFF_TIMER, EVT_COUNTER, ELSE } EVENTS; 
	enum { ACT_INIT, ACT_ON, ACT_OFF } ACTIONS; 
		
	// Custom class variables
	short pin;
	int repeat_count;
	atm_milli_timer on_timer, off_timer;
	atm_counter counter;

	// Methods used
	ATM_CLASSNAME & begin( int attached_pin );
	ATM_CLASSNAME & blink( int duration ); 
	ATM_CLASSNAME & pause( int duration ); 
	ATM_CLASSNAME & fade( int fade ); 
	ATM_CLASSNAME & repeat( int repeat ); 
	int event( int id );
	void action( int id );
};

#endif
