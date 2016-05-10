
#ifndef Atm_blink_h
#define Atm_blink_h

#include <Automaton.h>

class Atm_blink: public Machine {

  public:
    Atm_blink( void ) : Machine() { class_label = "BLNK"; };

    short pin;     
    atm_timer_millis timer;

    enum { IDLE, LED_ON, LED_OFF }; // STATES
    enum { EVT_TIMER, EVT_ON, EVT_OFF, ELSE }; // EVENTS
    enum { ACT_ON, ACT_OFF }; // ACTIONS
	
    Atm_blink & begin( int attached_pin, uint32_t blinkrate );
    Atm_blink & trace( Stream & stream );
    int event( int id ); 
    void action( int id ); 
};

#endif
