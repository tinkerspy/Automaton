#ifndef Atm_fade_h
#define Atm_fade_h

#include <Automaton.h>

#undef ATM_CLASSNAME
#define ATM_CLASSNAME Atm_fade

class ATM_CLASSNAME : public Machine {

	public:

		ATM_CLASSNAME( void ) : Machine( ) { class_label="FAD"; };
		
		enum { IDLE, ON, START, STARTU, UP, STARTD, DOWN, REPEAT } STATES;
		enum { EVT_CNT_FADE, EVT_TM_FADE, EVT_TM_ON, EVT_TM_OFF, EVT_CNT_RPT, ELSE, ACT_REPEAT, ACT_OFF, ACT_ON, ACT_UP, ACT_DOWN, ACT_START } IO;

		static const uint8_t SLOPE_SIZE = 32;
		uint8_t slope[SLOPE_SIZE] = { 	
			0,   0,   1,   1,   2,    2,  3,    4,   
			5,   6,   7,   8,  10,   12,  15,  18, 
			22,  26,  31,  37,  44,  54,  63,  76,  
			90, 108, 127, 153, 180, 217, 230, 255 };
		short pin;
		int repeat_count;
		atm_milli_timer timer_fade, timer_on, timer_off;
		atm_counter counter_fade, counter_repeat;
		
		ATM_CLASSNAME & begin( int attached_pin );
		ATM_CLASSNAME & blink( int duration ); 
		ATM_CLASSNAME & pause( int duration ); 
		ATM_CLASSNAME & fade( int fade ); 
		ATM_CLASSNAME & repeat( int repeat ); 
		int event( int id );
		void action( int id );
};

#endif
