
#ifndef Atm_timer_h
#define Atm_timer_h

#include <Automaton.h>

typedef void (*timer_cb_t)( int );

class Atm_timer: public Machine {

  public:
    Atm_timer( void ) : Machine() { class_label = "TMR"; };

    atm_milli_timer timer;
    atm_counter counter;
    int timer_id;
    timer_cb_t callback = 0;
    Machine * client_machine;
    uint8_t client_msg;

    enum { IDLE, WAIT, TRIGGER } STATES;
    enum { EVT_TIMER, EVT_COUNTER, EVT_OFF, EVT_ON, ELSE } EVENTS;
	  enum { ACT_TRIG } ACTIONS;
    enum { MSG_OFF, MSG_ON, MSG_END } MESSAGES;

    atm_msg_t messages[MSG_END];
	
    Atm_timer & begin( void );
    Atm_timer & onTimer( Machine * machine, uint8_t msg );
    Atm_timer & onTimer( timer_cb_t timer_callback ); 
    Atm_timer & interval( int v );
    Atm_timer & repeat( int v );
    Atm_timer & id( int v );
    int event( int id ); 
    void action( int id ); 
};

#endif
