
#ifndef blink_h
#define blink_h

#include <Automaton.h>

#undef ATM_CLASSNAME
#define ATM_CLASSNAME Blink

class ATM_CLASSNAME: public Machine {

  public:
    ATM_CLASSNAME( void ) : Machine() { class_label = "BLNK"; };

    short pin;     
    atm_timer timer;

    enum { LED_ON, LED_OFF } STATES;
    enum { EVT_TIMER, ELSE } EVENTS;
	enum { ACT_ON, ACT_OFF } ACTIONS;
	
    ATM_CLASSNAME & begin( int attached_pin, int blinkrate );
    int event( int id ); 
    void action( int id ); 
};

#endif
