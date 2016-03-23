
#ifndef Atm_timer_h
#define Atm_timer_h

#include <Automaton.h>

typedef void (*timer_cb_t)( int, uint16_t );

class Atm_timer: public Machine {

  public:
    Atm_timer( void ) : Machine() { class_label = "TMR"; };

    atm_timer daytimer, mstimer;
    atm_counter daycounter, repcounter;
    int timer_id;
    timer_cb_t callback = 0;
    Machine * client_machine;
    uint8_t client_msg;
    uint16_t days; 
    uint16_t repeat_cnt; 
    
    enum { IDLE, START, WAITD, WAITMS, TRIGGER } STATES;
    enum { EVT_DAYCNT, EVT_DAYTIMER, EVT_MSTIMER, EVT_REPCNT, EVT_OFF, EVT_ON, ELSE } EVENTS;
	  enum { ACT_START, ACT_TRIG, ACT_WAITD } ACTIONS;
    enum { MSG_OFF, MSG_ON, MSG_END } MESSAGES;

    atm_msg_t messages[MSG_END];
	
    Atm_timer & begin( void );
    Atm_timer & begin( uint32_t ms );
    Atm_timer & onSwitch( swcb_sym_t switch_callback );
    Atm_timer & onTimer( Machine * machine, uint8_t msg );
    Atm_timer & onTimer( timer_cb_t timer_callback ); 
    Atm_timer & interval_seconds( uint32_t v );
    Atm_timer & interval_millis( uint32_t v );
    Atm_timer & interval( uint32_t v );
    Atm_timer & repeat( int v );
    Atm_timer & id( int v );
    int event( int id ); 
    void action( int id ); 
};

// Tiny Machine version

class Att_timer: public TinyMachine {

  public:
    Att_timer( void ) : TinyMachine() { };

    atm_timer daytimer, mstimer;
    atm_counter daycounter, repcounter;
    int timer_id;
    timer_cb_t callback = 0;
    Machine * client_machine;
    uint8_t client_msg;
    uint16_t days; 
    uint16_t repeat_cnt; 
    
    enum { IDLE, START, WAITD, WAITMS, TRIGGER } STATES;
    enum { EVT_DAYCNT, EVT_DAYTIMER, EVT_MSTIMER, EVT_REPCNT, EVT_OFF, EVT_ON, ELSE } EVENTS;
    enum { ACT_START, ACT_TRIG, ACT_WAITD } ACTIONS;
	
    Att_timer & begin( void );
    Att_timer & begin( uint32_t ms );
    Att_timer & onTimer( Machine * machine, uint8_t msg );
    Att_timer & onTimer( timer_cb_t timer_callback ); 
    Att_timer & interval_seconds( uint32_t v );
    Att_timer & interval_millis( uint32_t v );
    Att_timer & interval( uint32_t v );
    Att_timer & repeat( int v );
    Att_timer & id( int v );
    int event( int id ); 
    void action( int id ); 
};



#endif
