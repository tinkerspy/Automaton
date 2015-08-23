
#ifndef Atm_teensywave_h
#define Atm_teensywave_h

#include <Automaton.h>

#undef ATM_CLASSNAME
#define ATM_CLASSNAME Atm_teensywave

class ATM_CLASSNAME: public Machine {

  public:
    ATM_CLASSNAME( void ) : Machine() { class_label = "WAVE"; };

    short pin;     
    atm_micro_timer timer;
    atm_counter phase;
    int _steps, _delay;
    float _stepsize;

    enum { IDLE, START_SN, SINE, START_SW, SAW, START_SQ, SQON, SQOFF } STATES;
    enum { EVT_COUNTER, EVT_TIMER, EVT_TOGGLE, ELSE } EVENTS;
    enum { ACT_IDLE, ACT_START, ACT_SINE, ACT_SAW, ACT_STARTSQ, ACT_SQON, ACT_SQOFF } ACTIONS;
	enum { SIG_TOGGLE } SIGNALS;
		
    ATM_CLASSNAME & begin( int attached_pin, int steps, int delay );
    int event( int id ); 
    void action( int id ); 
};

#endif