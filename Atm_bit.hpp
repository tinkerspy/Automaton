
#ifndef Atm_bit_h
#define Atm_bit_h

#include <Automaton.h>

typedef void (*bitcb_t)( int idx );

class Atm_Bit_Comm {
  public:
    uint8_t _mode = 0;
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


class Atm_bit: public Machine {

  public:
    Atm_bit( void ) : Machine() { class_label = "BIT"; };

    state_t _last_state;
    Atm_Bit_Comm _comm[4];

    enum { OFF, ON }; // STATES
    enum { EVT_ON, EVT_OFF, EVT_TOGGLE, EVT_INPUT, ELSE }; // EVENTS
    enum { ACT_ON, ACT_OFF, ACT_INPUT }; // ACTIONS
    enum { MODE_NULL, MODE_CALLBACK, MODE_MACHINE, MODE_TMACHINE, MODE_FACTORY };
  
    Atm_bit & begin( bool default_state = false );
    Atm_bit & onFlip( bool st, bitcb_t callback );
    Atm_bit & onFlip( bool st, Machine & machine, state_t event /* = 0 */ );
    Atm_bit & onFlip( bool st, const char * label, state_t event /* = 0 */ );
    Atm_bit & onFlip( bool st, TinyMachine & machine, state_t event /* = 0 */ );
    Atm_bit & onInput( bool st, bitcb_t callback );
    Atm_bit & onInput( bool st, Machine & machine, state_t event /* = 0 */ );
    Atm_bit & onInput( bool st, const char * label, state_t event /* = 0 */ );
    Atm_bit & onInput( bool st, TinyMachine & machine, state_t event /* = 0 */ );

    Atm_bit & trace( Stream & stream );
    int event( int id ); 
    void action( int id ); 
    void comm( Atm_Bit_Comm & c, state_t st );
};

// Tiny Machine version


class Att_bit: public TinyMachine {

  public:
    Att_bit( void ) : TinyMachine() {};

    state_t _last_state;
    Atm_Bit_Comm _comm[4];

    enum { OFF, ON }; // STATES
    enum { EVT_ON, EVT_OFF, EVT_TOGGLE, EVT_INPUT, ELSE }; // EVENTS
    enum { ACT_ON, ACT_OFF, ACT_INPUT }; // ACTIONS
    enum { MODE_NULL, MODE_CALLBACK, MODE_MACHINE, MODE_TMACHINE, MODE_FACTORY };
  
    Att_bit & begin( bool default_state = false );
    Att_bit & onFlip( bool st, bitcb_t callback );
    Att_bit & onFlip( bool st, Machine & machine, state_t event /* = 0 */ );
    Att_bit & onFlip( bool st, TinyMachine & machine, state_t event /* = 0 */ );
    Att_bit & onInput( bool st, bitcb_t callback );
    Att_bit & onInput( bool st, Machine & machine, state_t event /* = 0 */ );
    Att_bit & onInput( bool st, TinyMachine & machine, state_t event /* = 0 */ );

    Att_bit & trace( Stream & stream );
    int event( int id ); 
    void action( int id ); 
    void comm( Atm_Bit_Comm & c, state_t st );
};


#endif

