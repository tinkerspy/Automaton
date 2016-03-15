
#ifndef Atm_blink_h
#define Atm_blink_h

#include <Automaton.h>

class Atm_blink: public Machine {

  public:
    Atm_blink( void ) : Machine() { class_label = "BLNK"; };

    short pin;     
    atm_timer_millis timer;

    enum { LED_ON, LED_OFF } STATES;
    enum { EVT_TIMER, ELSE } EVENTS;
	  enum { ACT_ON, ACT_OFF } ACTIONS;
	
    Atm_blink & begin( int attached_pin, int blinkrate );
    Atm_blink & onSwitch( swcb_sym_t switch_callback );
    int event( int id ); 
    void action( int id ); 
};

#endif
