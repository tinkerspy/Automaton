
#ifndef Atm_led_h
#define Atm_led_h

#include <Automaton.h>

class Atm_led : public Machine {

  public:
  	// Class constructor
	Atm_led( void ) : Machine() { class_label = "LED"; };

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
	Atm_led & begin( int attached_pin );
	Atm_led & blink( int duration ); 
	Atm_led & pause( int duration ); 
	Atm_led & fade( int fade ); 
	Atm_led & repeat( int repeat ); 
	int event( int id );
	void action( int id );
};

#endif
