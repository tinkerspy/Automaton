
#ifndef Atm_condition_h
#define Atm_condition_h

#include <Automaton.h>

typedef void (*condcb_t)( int idx );

class Atm_Condition_Comm {
  public:
    uint8_t _mode = 0;
    char _logop, _relop;
    union {
      struct { // ATM_USR1_FLAG - callback
        void (*_callback)( state_t st );
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
};


class Atm_condition: public Machine {

  public:
    Atm_condition( void ) : Machine() { class_label = "CON"; };

    state_t _last_state;
    Atm_Condition_Comm _comm[4];
    Atm_Condition_Comm _op[4];

    enum { OFF, ON }; // STATES
    enum { EVT_ON, EVT_OFF, EVT_TOGGLE, EVT_INPUT, ELSE }; // EVENTS
    enum { ACT_ON, ACT_OFF, ACT_INPUT }; // ACTIONS
    enum { MODE_NULL, MODE_CALLBACK, MODE_MACHINE, MODE_TMACHINE, MODE_FACTORY };
  
    Atm_condition & begin( bool default_state = false );
    Atm_condition & onFlip( bool st, condcb_t callback );
    Atm_condition & onFlip( bool st, Machine & machine, state_t event = 0);
    Atm_condition & onFlip( bool st, const char * label, state_t event = 0 );
    Atm_condition & onFlip( bool st, TinyMachine & machine, state_t event = 0 );
    Atm_condition & onInput( bool st, condcb_t callback );
    Atm_condition & onInput( bool st, Machine & machine, state_t event = 0 );
    Atm_condition & onInput( bool st, const char * label, state_t event = 0 );
    Atm_condition & onInput( bool st, TinyMachine & machine, state_t event = 0 );
    Atm_condition & IF(      Machine & machine, char relOp = '>', state_t match = 0 );
    Atm_condition & IF(  TinyMachine & machine, char relOp = '>', state_t match = 0 );
    Atm_condition & AND(     Machine & machine, char relOp = '>', state_t match = 0 );
    Atm_condition & AND( TinyMachine & machine, char relOp = '>', state_t match = 0 );
    Atm_condition & OR(      Machine & machine, char relOp = '>', state_t match = 0 );
    Atm_condition & OR(  TinyMachine & machine, char relOp = '>', state_t match = 0 );

    Atm_condition & trace( Stream & stream );
  private:
    int event( int id ); 
    void action( int id ); 
    void comm( Atm_Condition_Comm & c, state_t st );
    Atm_condition & OP( char logOp, Machine & machine, char relOp, state_t match );
    Atm_condition & OP( char logOp, TinyMachine & machine, char relOp, state_t match );
    bool eval_one( uint8_t idx );
    bool eval();

};

#endif

