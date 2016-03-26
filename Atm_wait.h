#ifndef Atm_wait_h
#define Atm_wait_h

#include <Automaton.h>

class Atm_wait: public Machine {

  public:
    Atm_wait( void ) : Machine() { class_label = "WAIT"; };

    atm_timer_millis timer;

    enum { IDLE, WAIT };
    enum { EVT_STOP, EVT_TIMER, ELSE };

    Atm_wait & begin( uint32_t period );
    Atm_wait & onSwitch( swcb_sym_t switch_callback );
    int event( int id );
    void action( int id );
};

class Att_wait: public TinyMachine {

  public:
    Att_wait( void ) : TinyMachine() {};

    atm_timer_millis timer;

    enum { IDLE, WAIT };
    enum { EVT_STOP, EVT_TIMER, ELSE };

    Att_wait & begin( uint32_t period );
    int event( int id );
    void action( int id );
};

#endif^M

