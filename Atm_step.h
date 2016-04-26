
#ifndef Atm_step_h
#define Atm_step_h

#include <Automaton.h>

typedef void (*stepcb_t)( int idx );

class Step {
  public:
    uint8_t _mode = 0;
    union {
      struct { // ATM_USR1_FLAG - callback
        void (*_callback)( int idx );
      };
      struct { // ATM_USR2_FLAG - machine trigger
        Machine * _client_machine;
        state_t _client_machine_event;
      };
      struct { // ATM_USR3_FLAG - factory trigger
        const char * _client_label;
        state_t _client_label_event;
      };
    };
};

class Atm_step: public Machine {

  public:
    Atm_step( void ) : Machine() {
      class_label = "STP";
    };
    bool _sweep = false;
    Step _step[8];
    enum { IDLE, START, S0, S1, S2, S3, S4, S5, S6, S7, X0, X1, X2, X3, X4, X5, X6, X7, X8, X9, XA, XB, XC, XD };
    enum { EVT_NEXT, EVT_SWEEP, EVT_RESET, ELSE };
    enum { ACT_S0, ACT_S1, ACT_S2, ACT_S3, ACT_S4, ACT_S5, ACT_S6, ACT_S7 };
    enum { MODE_NULL, MODE_CALLBACK, MODE_MACHINE, MODE_FACTORY };

    Atm_step & begin( void );
    Atm_step & sweep( void );
    Atm_step & linear( void );
    Atm_step & trace( Stream * stream );
    int event( int id );
    void action( int id );
    Atm_step & onStep( uint8_t idx, stepcb_t callback );
    Atm_step & onStep( uint8_t idx, Machine * machine, state_t event );
    Atm_step & onStep( uint8_t idx, const char * label, state_t event );
};

#endif
