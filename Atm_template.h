#ifndef Atm_template_h
#define Atm_template_h

#undef ATM_CLASSNAME
#define ATM_CLASSNAME Atm_template

#include <Automaton.h>

class ATM_CLASSNAME : public Machine {

  public:
	// Constructor, leave this unchanged
	ATM_CLASSNAME( void ) : Machine() { class_label="TPL"; };
 	
	// Your state, event & action names, customize this 
	enum { IDLE, STATE1, STATE2 } STATES; // In state table row order
	enum { EVT_TIMER, ELSE, ACT_ONE, ACT_TWO } IO; // Custom event & io constants

	// Your custom variables go here 
	static const uint32_t TIMER = 1000;
	atm_timer timer;
	
	// Prototypes for your methods, minimum: begin(), event() & action()
	Machine & begin( void );
	int event( int id );
	void action( int id );
};
	
#endif
