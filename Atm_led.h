
#ifndef Atm_led_h
#define Atm_led_h

#include <Automaton.h>

class Atm_led : public Machine {

  public:
  	// Class constructor
	Atm_led( void ) : Machine() { class_label = "LED"; };

	// Custom state, event, action & message constants (enum)
	enum { IDLE, ON, START, BLINK_OFF, DONE }; 
	enum { EVT_ON_TIMER, EVT_OFF_TIMER, EVT_COUNTER, EVT_ON, EVT_OFF, EVT_BLINK, ELSE }; 
	enum { ACT_INIT, ACT_ON, ACT_OFF, ACT_CHAIN }; 
    enum { MSG_ON, MSG_OFF, MSG_BLINK, MSG_END };
		
	// Custom class variables
    atm_msg_t messages[MSG_END];
	short pin;
	int repeat_count;
	atm_timer_millis on_timer, off_timer;
	atm_counter counter;
    Machine * chain_next;
    Machine * chain_previous;
    uint8_t chain_event;

	// Methods used
	Atm_led & begin( int attached_pin );
	Atm_led & blink( uint32_t duration ); 
	Atm_led & pause( uint32_t duration ); 
	Atm_led & fade( int fade ); 
	Atm_led & repeat( int repeat ); 
    Atm_led & onSwitch( swcb_sym_t switch_callback );
    Atm_led & chain( Machine * n = 0, Machine * p = 0, uint8_t event = EVT_BLINK );
	int event( int id );
	void action( int id );
};


class Att_led : public TinyMachine {

  public:
  	// Class constructor
	Att_led( void ) : TinyMachine() { };

	// Custom state, event, action & message constants (enum)
	enum { IDLE, ON, START, BLINK_OFF, DONE }; 
	enum { EVT_ON_TIMER, EVT_OFF_TIMER, EVT_COUNTER, EVT_ON, EVT_OFF, EVT_BLINK, ELSE }; 
	enum { ACT_INIT, ACT_ON, ACT_OFF, ACT_CHAIN }; 
		
	// Custom class variables
	short pin;
	int repeat_count;
	atm_timer_millis on_timer, off_timer;
	atm_counter counter;
    TinyMachine * chain_next;
    TinyMachine * chain_previous;
    uint8_t chain_event;

	// Methods used
	Att_led & begin( int attached_pin );
	Att_led & blink( uint32_t duration ); 
	Att_led & pause( uint32_t duration ); 
	Att_led & fade( int fade ); 
	Att_led & repeat( int repeat ); 
    Att_led & onSwitch( swcb_sym_t switch_callback );
    Att_led & chain( TinyMachine * n = 0, TinyMachine * p = 0, uint8_t event = EVT_BLINK );
	int event( int id );
	void action( int id );
};

#endif
