
#ifndef Atm_timer_h
#define Atm_timer_h

#include <Automaton.h>

typedef void (*timer_cb_t)( int, uint16_t );

class Atm_timer: public Machine {

  public:
    Atm_timer( void ) : Machine() { class_label = "TMR"; };

    atm_timer_millis daytimer, mstimer;
    atm_counter daycounter, repcounter;
    uint16_t days; 
    uint16_t repeat_cnt; 
    union {
      struct { // ATM_USR1_FLAG - callback
        void (*_callback)( int idx, uint16_t cnt );
        int _callback_idx;
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
    
    enum { IDLE, START, WAITD, WAITMS, TRIGGER };
    enum { EVT_DAYCNT, EVT_DAYTIMER, EVT_MSTIMER, EVT_REPCNT, EVT_OFF, EVT_ON, ELSE };
    enum { ACT_START, ACT_TRIG, ACT_WAITD };
	
    Atm_timer & begin( uint32_t ms = 0 );
    Atm_timer & trace( Stream & stream );
    Atm_timer & onTimer( timer_cb_t callback, int idx = 0 );
    Atm_timer & onTimer( Machine & machine, int event = 0 );
    Atm_timer & onTimer( TinyMachine & machine, int event = 0 );
    Atm_timer & onTimer( const char * label, int event = 0 );
    Atm_timer & interval_seconds( uint32_t v );
    Atm_timer & interval_millis( uint32_t v );
    Atm_timer & interval( uint32_t v );
    Atm_timer & repeat( uint16_t v );
    int event( int id ); 
    void action( int id ); 
};

// Tiny Machine version

class Att_timer: public TinyMachine {

  public:
    Att_timer( void ) : TinyMachine() {};

    atm_timer_millis daytimer, mstimer;
    atm_counter daycounter, repcounter;
    uint16_t days; 
    uint16_t repeat_cnt; 
    union {
      struct { // ATM_USR1_FLAG - callback
        void (*_callback)( int idx, uint16_t cnt );
        int _callback_idx;
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
    
    enum { IDLE, START, WAITD, WAITMS, TRIGGER };
    enum { EVT_DAYCNT, EVT_DAYTIMER, EVT_MSTIMER, EVT_REPCNT, EVT_OFF, EVT_ON, ELSE };
    enum { ACT_START, ACT_TRIG, ACT_WAITD };
	
    Att_timer & begin( uint32_t ms = 0 );
    Att_timer & trace( Stream & stream );
    Att_timer & onTimer( timer_cb_t callback, int idx = 0 );
    Att_timer & onTimer( Machine & machine, int event = 0 );
    Att_timer & onTimer( TinyMachine & machine, int event = 0 );
    Att_timer & interval_seconds( uint32_t v );
    Att_timer & interval_millis( uint32_t v );
    Att_timer & interval( uint32_t v );
    Att_timer & repeat( uint16_t v );
    int event( int id ); 
    void action( int id ); 
};




#endif
