#ifndef Atm_fade_h
#define Atm_fade_h

#include <Automaton.h>

class Atm_fade : public Machine {

	public:

		Atm_fade( void ) : Machine( ) { class_label="FAD"; };
		
		enum { IDLE, ON, START, STARTU, UP, STARTD, DOWN, REPEAT } STATES;
		enum { EVT_CNT_FADE, EVT_TM_FADE, EVT_TM_ON, EVT_TM_OFF, EVT_CNT_RPT, EVT_ON, EVT_OFF, EVT_BLINK, ELSE } EVENTS;
        enum { ACT_REPEAT, ACT_OFF, ACT_ON, ACT_UP, ACT_DOWN, ACT_START } ACTIONS;
        enum { MSG_ON, MSG_OFF, MSG_BLINK, MSG_END } MESSAGES;
		
        atm_msg_t messages[MSG_END];
    
		static const uint8_t SLOPE_SIZE = 32;
		uint8_t slope[SLOPE_SIZE] = { 	
			0,   0,   1,   1,   2,    2,  3,    4,   
			5,   6,   7,   8,  10,   12,  15,  18, 
			22,  26,  31,  37,  44,  54,  63,  76,  
			90, 108, 127, 153, 180, 217, 230, 255 };
		short pin;
		int repeat_count;
		atm_timer_millis timer_fade, timer_on, timer_off;
		atm_counter counter_fade, counter_repeat;
		
		Atm_fade & begin( int attached_pin );
		Atm_fade & blink( int duration ); 
		Atm_fade & pause( int duration ); 
		Atm_fade & fade( int fade ); 
		Atm_fade & repeat( int repeat ); 
		int event( int id );
		void action( int id );
};

// TinyMachine version

class Att_fade : public TinyMachine {

	public:

		Att_fade( void ) : TinyMachine( ) { };
		
		enum { IDLE, ON, START, STARTU, UP, STARTD, DOWN, REPEAT } STATES;
		enum { EVT_CNT_FADE, EVT_TM_FADE, EVT_TM_ON, EVT_TM_OFF, EVT_CNT_RPT, EVT_ON, EVT_OFF, EVT_BLINK, ELSE } EVENTS;
        enum { ACT_REPEAT, ACT_OFF, ACT_ON, ACT_UP, ACT_DOWN, ACT_START } ACTIONS;
		    
		static const uint8_t SLOPE_SIZE = 32;
		uint8_t slope[SLOPE_SIZE] = { 	
			0,   0,   1,   1,   2,    2,  3,    4,   
			5,   6,   7,   8,  10,   12,  15,  18, 
			22,  26,  31,  37,  44,  54,  63,  76,  
			90, 108, 127, 153, 180, 217, 230, 255 };
		short pin;
		int repeat_count;
		atm_timer_millis timer_fade, timer_on, timer_off;
		atm_counter counter_fade, counter_repeat;
		
		Att_fade & begin( int attached_pin );
		Att_fade & blink( int duration ); 
		Att_fade & pause( int duration ); 
		Att_fade & fade( int fade ); 
		Att_fade & repeat( int repeat ); 
		int event( int id );
		void action( int id );
};

#endif
