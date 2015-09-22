
#ifndef Atm_teensywave_h
#define Atm_teensywave_h

#include <Automaton.h>

class Atm_teensywave: public Machine {

  public:
    Atm_teensywave( void ) : Machine() { class_label = "WAVE"; };

    short pin;     
    atm_timer_micros timer;
    atm_counter phase;
    uint16_t _steps, _delay;
    float _stepsize;

    enum { IDLE, START_SN, SINE, START_SW, SAW, START_SR, SAWR, START_TR, TRI, START_SQ, SQON, SQOFF } STATES;
    enum { EVT_COUNTER, EVT_TIMER, EVT_TOGGLE, ELSE } EVENTS;
    enum { ACT_IDLE, ACT_START, ACT_SINE, ACT_SAW, ACT_SAWR, ACT_STARTTR, ACT_TRI, ACT_STARTSQ, ACT_SQON, ACT_SQOFF } ACTIONS;
    enum { MSG_TOGGLE, MSG_END } MESSAGES;
    atm_msg_t messages[MSG_END];
			
    Atm_teensywave & begin( int attached_pin, int steps, int delay );
    int event( int id ); 
    void action( int id ); 
};

#endif

