
#ifndef Atm_button_h
#define Atm_button_h

#include <Automaton.h>

typedef void (*presscb_t)( int press );

class Atm_button : public Machine {

  public:
	Atm_button( void ) : Machine() { class_label = "BTN"; };
 
	enum { IDLE, WAIT, PRESSED, REPEAT, RELEASE, LIDLE, LWAIT, LPRESSED, LRELEASE, WRELEASE, AUTO } STATES;
	enum { EVT_LMODE, EVT_TIMER, EVT_DELAY, EVT_REPEAT, EVT_PRESS, EVT_RELEASE, EVT_COUNTER, EVT_AUTO, ELSE } EVENTS;
	enum { ACT_PRESS, ACT_RELEASE, ACT_LSTART, ACT_LCOUNT, ACT_LRELEASE, ACT_WRELEASE, ACT_AUTO } ACTIONS;
	
	static const int DEBOUNCE = 5;
	short pin;
	atm_timer_millis timer_debounce, timer_delay, timer_repeat, timer_auto;
	atm_counter counter_longpress;
	void (*callback)( int press ) = 0;
	int16_t longpress_max;
    int16_t _auto_press = 1;
    Machine * client_machine;
    int client_press = -1;
    int client_release = -1;
    
	Atm_button & begin( int attached_pin, presscb_t press_callback );
	Atm_button & begin( int attached_pin );
    Atm_button & onPress( Machine * machine, int msg ); 
    Atm_button & onPress( Machine * machine, int msg_press, int msg_release );
    Atm_button & onPress( presscb_t press_callback );
	Atm_button & debounce( int delay );
	Atm_button & longPress( int max, int delay );
	Atm_button & repeat( int delay, int speed );
	Atm_button & repeat( void );
    Atm_button & autoPress( int delay, int press );
	int event( int id );
	void action( int id );
};
	
#endif
