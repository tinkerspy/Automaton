
#ifndef Atm_button_h
#define Atm_button_h

#include <Automaton.h>

typedef void (*presscb_t)( int press );
typedef void (*presscb_id_t)( int press, int id );

class Atm_button : public Machine {

  public:
	Atm_button( void ) : Machine() { class_label = "BTN"; };
 
	enum { IDLE, WAIT, PRESSED, REPEAT, RELEASE, LIDLE, LWAIT, LPRESSED, LRELEASE, WRELEASE, AUTO };
	enum { EVT_LMODE, EVT_TIMER, EVT_DELAY, EVT_REPEAT, EVT_PRESS, EVT_RELEASE, EVT_COUNTER, EVT_AUTO, ELSE };
	enum { ACT_PRESS, ACT_RELEASE, ACT_LSTART, ACT_LCOUNT, ACT_LRELEASE, ACT_WRELEASE, ACT_AUTO };
	
	static const int DEBOUNCE = 5;
	short pin;
	atm_timer_millis timer_debounce, timer_delay, timer_repeat, timer_auto;
	atm_counter counter_longpress;
	void (*callback)( int press ) = 0;
	void (*callback_id)( int press, int id ) = 0;
    int callback_idx;
	int16_t longpress_max;
    int16_t _auto_press = 1;
    Machine * client_machine;
    int client_press = -1;
    int client_release = -1;
    
	Atm_button & begin( int attached_pin, presscb_t press_callback );
	Atm_button & begin( int attached_pin );
    Atm_button & onSwitch( swcb_sym_t switch_callback );
    Atm_button & onPress( Machine * machine, int msg ); 
    Atm_button & onPress( Machine * machine, int msg_press, int msg_release );
    Atm_button & onPress( presscb_t press_callback );
    Atm_button & onPress( presscb_id_t press_callback, int idx );
	Atm_button & debounce( int delay );
	Atm_button & longPress( int max, int delay );
	Atm_button & repeat( int delay, int speed );
	Atm_button & repeat( void );
    Atm_button & autoPress( int delay, int press );
    void cb( int press, int id );
	int event( int id );
	void action( int id );
};

class Att_button : public TinyMachine {

  public:
	Att_button( void ) : TinyMachine() { };
 
	enum { IDLE, WAIT, PRESSED, REPEAT, RELEASE, LIDLE, LWAIT, LPRESSED, LRELEASE, WRELEASE, AUTO };
	enum { EVT_LMODE, EVT_TIMER, EVT_DELAY, EVT_REPEAT, EVT_PRESS, EVT_RELEASE, EVT_COUNTER, EVT_AUTO, ELSE };
	enum { ACT_PRESS, ACT_RELEASE, ACT_LSTART, ACT_LCOUNT, ACT_LRELEASE, ACT_WRELEASE, ACT_AUTO };
	
	static const int DEBOUNCE = 5;
	short pin;
	atm_timer_millis timer_debounce, timer_delay, timer_repeat, timer_auto;
	atm_counter counter_longpress;
	void (*callback)( int press ) = 0;
	void (*callback_id)( int press, int id ) = 0;
    int callback_idx;
	int16_t longpress_max;
    int16_t _auto_press = 1;
    Machine * client_machine;
    int client_press = -1;
    int client_release = -1;
    
	Att_button & begin( int attached_pin, presscb_t press_callback );
	Att_button & begin( int attached_pin );
    Att_button & onPress( Machine * machine, int msg ); 
    Att_button & onPress( Machine * machine, int msg_press, int msg_release );
    Att_button & onPress( presscb_t press_callback );
    Att_button & onPress( presscb_id_t press_callback, int idx );
	Att_button & debounce( int delay );
	Att_button & longPress( int max, int delay );
	Att_button & repeat( int delay, int speed );
	Att_button & repeat( void );
    Att_button & autoPress( int delay, int press );
    void cb( int press, int id );
	int event( int id );
	void action( int id );
};	
#endif
