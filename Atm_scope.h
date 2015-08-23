
#ifndef Atm_scope_h
#define Atm_scope_h

#include <Automaton.h>

#undef ATM_CLASSNAME
#define ATM_CLASSNAME Atm_scope

class ATM_CLASSNAME: public Machine {

  public:
    ATM_CLASSNAME( void ) : Machine() { class_label = "SCP"; };

    short pin;     

    enum { LED_ON, LED_OFF } STATES;
    enum { ELSE } EVENTS;
	enum { ACT_ON, ACT_OFF } ACTIONS;
	
    ATM_CLASSNAME & begin( int attached_pin );
    int event( int id ); 
    void action( int id ); 
};

#endif
