
#ifndef Atm_gate_h
#define Atm_gate_h

#include <Automaton.h>

typedef void (*opencb_t)( int idx, uint16_t cnt );

class Atm_gate: public Machine {

  public:
    Atm_gate( void ) : Machine() {
      class_label = "GT";
    };

    enum { IDLE, G0, G1, G2, G3, G4, G5, G6, G7, CHECK, OPEN, CLEAR };
    enum { EVT_G0, EVT_G1, EVT_G2, EVT_G3, EVT_G4, EVT_G5, EVT_G6, EVT_G7, EVT_OPEN, EVT_CLEAR, ELSE };
    enum { ACT_G0, ACT_G1, ACT_G2, ACT_G3, ACT_G4, ACT_G5, ACT_G6, ACT_G7, ACT_CLEAR, ACT_OPEN };

    Atm_gate & begin( int count );
    Atm_gate & trace( Stream & stream );
    Atm_gate & onOpen( opencb_t callback, int idx = 0 );
    Atm_gate & onOpen( Machine & machine, int event = 0 );
    Atm_gate & onOpen( TinyMachine & machine, int event = 0 );
    Atm_gate & onOpen( const char * label, int event = 0 );

  protected:
    uint8_t _gates;
    uint8_t _gate_count;

    union {
      struct { // ATM_USR1_FLAG - callback
        void (*_callback)( int idx, uint16_t cnt );
        int _callback_idx;
        uint16_t _callback_count;
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

    int event( int id );
    void action( int id );
    void gates_init( int count );
    void gate_clear( int id );
};


// Tiny Machine version

class Att_gate: public TinyMachine {

  public:
    Att_gate( void ) : TinyMachine() {
    };

    enum { IDLE, G0, G1, G2, G3, G4, G5, G6, G7, CHECK, OPEN, CLEAR };
    enum { EVT_G0, EVT_G1, EVT_G2, EVT_G3, EVT_G4, EVT_G5, EVT_G6, EVT_G7, EVT_OPEN, EVT_CLEAR, ELSE };
    enum { ACT_G0, ACT_G1, ACT_G2, ACT_G3, ACT_G4, ACT_G5, ACT_G6, ACT_G7, ACT_CLEAR, ACT_OPEN };

    Att_gate & begin( int count );
    Att_gate & trace( Stream & stream );
    Att_gate & onOpen( opencb_t callback, int idx = 0 );
    Att_gate & onOpen( TinyMachine & machine, int event = 0 );
    Att_gate & onOpen( Machine & machine, int event = 0 );

  protected:
    uint8_t _gates;
    uint8_t _gate_count;

    union {
      struct { // ATM_USR1_FLAG - callback
        void (*_callback)( int idx, uint16_t cnt );
        int _callback_idx;
        uint16_t _callback_count;
      };
      struct { // ATM_USR2_FLAG - machine trigger
        Machine * _client_machine;
        state_t _client_machine_event;
      };
      struct { // ATM_USR4_FLAG - Tiny machine trigger
        TinyMachine * _client_tmachine;
        state_t _client_tmachine_event;
      };
    };

    int event( int id );
    void action( int id );
    void gates_init( int count );
    void gate_clear( int id );
};


#endif
