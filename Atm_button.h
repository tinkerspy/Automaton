
#ifndef Atm_button_h
#define Atm_button_h

#include <Automaton.h>

#undef ATM_CLASSNAME
#define ATM_CLASSNAME Atm_button

typedef void (*presscb_t)( int press );

class ATM_CLASSNAME : public Machine {

  public:
	ATM_CLASSNAME( void ) : Machine() { class_label = "BTN"; };
 
	enum { IDLE, WAIT, PRESSED, REPEAT, RELEASE, LIDLE, LWAIT, LPRESSED, LRELEASE, WRELEASE } STATES;
	enum { EVT_LMODE, EVT_TIMER, EVT_DELAY, EVT_REPEAT, EVT_PRESS, EVT_RELEASE, EVT_COUNTER, ELSE } EVENTS;
	enum { ACT_PRESS, ACT_RELEASE, ACT_LSTART, ACT_LCOUNT, ACT_LRELEASE, ACT_WRELEASE } ACTIONS;
	
	static const int DEBOUNCE = 5;
	short pin;
	atm_milli_timer timer_debounce, timer_delay, timer_repeat;
	atm_counter counter_longpress;
	void (*callback)( int press ) = 0;
	int16_t longpress_max;

	ATM_CLASSNAME & begin( int attached_pin, presscb_t press_callback );
	ATM_CLASSNAME & begin( int attached_pin, presscb_t press_callback, int pulldown );
	ATM_CLASSNAME & debounce( int delay );
	ATM_CLASSNAME & longPress( int max, int delay );
	ATM_CLASSNAME & repeat( int delay, int speed );
	ATM_CLASSNAME & repeat( void );
	int event( int id );
	void action( int id );
};
	
#endif
