
#ifndef Atm_button_h
#define Atm_button_h

#include <Automaton.h>

typedef void (*presscb_t)( int press, int idx, uint16_t cnt );

class Atm_button : public Machine {

  public:
	Atm_button( void ) : Machine() { class_label = "BTN"; };
 
	enum { IDLE, WAIT, PRESSED, REPEAT, RELEASE, LIDLE, LWAIT, LPRESSED, LRELEASE, WRELEASE, AUTO };
	enum { EVT_LMODE, EVT_TIMER, EVT_DELAY, EVT_REPEAT, EVT_PRESS, EVT_RELEASE, EVT_COUNTER, EVT_AUTO, ELSE };
	enum { ACT_PRESS, ACT_RELEASE, ACT_LSTART, ACT_LCOUNT, ACT_LRELEASE, ACT_WRELEASE, ACT_AUTO };
	enum { BTN_PASS4 = -2, BTN_PASS3 = -3, BTN_PASS2 = -2, BTN_PASS1 = -1, 
            BTN_RELEASE = 0, BTN_PRESS1 = 1, BTN_PRESS2 = 2, BTN_PRESS3 = 3, BTN_PRESS4 = 4 };
        
	Atm_button & begin( int attached_pin );
    Atm_button & trace( Stream & stream );
	Atm_button & onPress( presscb_t callback, int idx = 0 );
    Atm_button & onPress( Machine & machine, int event = 0 ); 
    Atm_button & onPress( TinyMachine & machine, int event = 0 ); 
    Atm_button & onPress( const char * label, int event = 0 ); 
	Atm_button & debounce( int delay );
	Atm_button & longPress( int max, int delay );
	Atm_button & repeat( int delay = 500, int speed = 50 );
    Atm_button & autoPress( int delay, int press = 1 );

  protected:	
	int event( int id );
	void action( int id );
    void cb( int press, int id );
	static const int _DEBOUNCE = 5;
	short _pin;
	atm_timer_millis _timer_debounce, _timer_delay, _timer_repeat, _timer_auto;
	atm_counter _counter_longpress;
    union {
        struct { // ATM_USR1_FLAG - callback
            void (*_callback)( int press, int idx, uint16_t cnt );
            int _callback_idx;
    		uint16_t _press_count;
	    };
        struct { // ATM_USR2_FLAG - machine trigger
            Machine * _client_machine;
            state_t _client_machine_event;
        };
        struct { // ATM_USR3_FLAG - factory trigger
            const char * _client_label;
            state_t _client_label_event;
        };
        struct { // ATM_USR4_FLAG - Tiny machine trigger
            TinyMachine * _client_tmachine;
            state_t _client_tmachine_event;
        };
    };
	int16_t _longpress_max;
    int16_t _auto_press = 1;
};

class Att_button : public TinyMachine {

  public:
	Att_button( void ) : TinyMachine() { };
 
	enum { IDLE, WAIT, PRESSED, REPEAT, RELEASE, LIDLE, LWAIT, LPRESSED, LRELEASE, WRELEASE, AUTO };
	enum { EVT_LMODE, EVT_TIMER, EVT_DELAY, EVT_REPEAT, EVT_PRESS, EVT_RELEASE, EVT_COUNTER, EVT_AUTO, ELSE };
	enum { ACT_PRESS, ACT_RELEASE, ACT_LSTART, ACT_LCOUNT, ACT_LRELEASE, ACT_WRELEASE, ACT_AUTO };
	enum { BTN_PASS4 = -2, BTN_PASS3 = -3, BTN_PASS2 = -2, BTN_PASS1 = -1, 
            BTN_RELEASE = 0, BTN_PRESS1 = 1, BTN_PRESS2 = 2, BTN_PRESS3 = 3, BTN_PRESS4 = 4 };
	
	static const int DEBOUNCE = 5;
	short pin;
	atm_timer_millis timer_debounce, timer_delay, timer_repeat, timer_auto;
	atm_counter counter_longpress;
    union {
        struct { // ATM_USR1_FLAG - callback
            void (*_callback)( int press, int idx, uint16_t cnt );
            int callback_idx;
            uint16_t _press_count;
        };
        struct { // ATM_USR2_FLAG - machine trigger
            TinyMachine * _client_machine;
            state_t client_machine_event;
        };
    };
	int16_t longpress_max;
    int16_t _auto_press = 1;
    
	Att_button & begin( int attached_pin );
	Att_button & onPress( presscb_t callback, int idx = 0 );
    Att_button & onPress( TinyMachine & machine, int event = 0 ); 
	Att_button & debounce( int delay );
	Att_button & longPress( int max, int delay );
	Att_button & repeat( int delay = 500, int speed = 50 );
    Att_button & autoPress( int delay, int press = 1 );
    void cb( int press, int id );
	int event( int id );
	void action( int id );
};	
#endif
