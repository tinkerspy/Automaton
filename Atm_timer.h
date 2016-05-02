
#ifndef Atm_timer_h
#define Atm_timer_h

#include <Automaton.h>

typedef void (*timer_cb_t)( int, uint16_t );

class Atm_timer: public Machine {

  public:
    Atm_timer( void ) : Machine() { class_label = "TMR"; };

    atm_timer_millis daytimer, mstimer;
    atm_counter daycounter, repcounter;
    int timer_id;
    timer_cb_t callback = 0;
    Machine * client_machine;
    uint8_t client_event;
    uint16_t days; 
    uint16_t repeat_cnt; 
    
    enum { IDLE, START, WAITD, WAITMS, TRIGGER };
    enum { EVT_DAYCNT, EVT_DAYTIMER, EVT_MSTIMER, EVT_REPCNT, EVT_OFF, EVT_ON, ELSE };
    enum { ACT_START, ACT_TRIG, ACT_WAITD };
	
    Atm_timer & begin( uint32_t ms = ATM_TIMER_OFF );
    Atm_timer & trace( Stream & stream );
    Atm_timer & onTimer( Machine & machine, uint8_t event );
    Atm_timer & onTimer( timer_cb_t timer_callback ); 
    Atm_timer & interval_seconds( uint32_t v );
    Atm_timer & interval_millis( uint32_t v );
    Atm_timer & interval( uint32_t v );
    Atm_timer & repeat( uint16_t v );
    Atm_timer & id( int v );
    int event( int id ); 
    void action( int id ); 
};

// Tiny Machine version

class Att_timer: public TinyMachine {

  public:
    Att_timer( void ) : TinyMachine() { };

    atm_timer_millis daytimer, mstimer;
    atm_counter daycounter, repcounter;
    int timer_id;
    timer_cb_t callback = 0;
    TinyMachine * client_machine;
    uint8_t client_event;
    uint16_t days; 
    uint16_t repeat_cnt; 
    
    enum { IDLE, START, WAITD, WAITMS, TRIGGER };
    enum { EVT_DAYCNT, EVT_DAYTIMER, EVT_MSTIMER, EVT_REPCNT, EVT_OFF, EVT_ON, ELSE };
    enum { ACT_START, ACT_TRIG, ACT_WAITD };
	
    Att_timer & begin( uint32_t ms = ATM_TIMER_OFF );
    Att_timer & onTimer( TinyMachine & machine, uint8_t event );
    Att_timer & onTimer( timer_cb_t timer_callback ); 
    Att_timer & interval_seconds( uint32_t v );
    Att_timer & interval_millis( uint32_t v );
    Att_timer & interval( uint32_t v );
    Att_timer & repeat( uint16_t v );
    Att_timer & id( int v );
    int event( int id ); 
    void action( int id ); 
};



#endif
