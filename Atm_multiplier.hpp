
#ifndef Atm_multiplier_h
#define Atm_multiplier_h

#include <Automaton.h>

typedef void ( *multicb_t )( void );

#define ATM_MULTIPLIER_COMMS_MAX 4

class Atm_Multiplier_Comm {
 public:
  uint8_t _mode = 0;
  union {
    struct {  // ATM_USR1_FLAG - callback
      void ( *_callback )( void );
    };
    struct {  // ATM_USR2_FLAG - machine trigger
      Machine* _client_machine;
      state_t _client_machine_event;
    };
    struct {  // ATM_USR3_FLAG - factory trigger
      const char* _client_label;
      state_t _client_label_event;
    };
    struct {  // ATM_USR4_FLAG - Tiny machine trigger
      TinyMachine* _client_tmachine;
      state_t _client_tmachine_event;
    };
  };
};

class Atm_multiplier : public Machine {
 public:
  Atm_multiplier( void ) : Machine() {
    class_label = "MUL";
  };

  Atm_Multiplier_Comm _comm[ATM_MULTIPLIER_COMMS_MAX];

  enum { IDLE, SEND };       // STATES
  enum { EVT_INPUT, ELSE };  // EVENTS
  enum { ACT_SEND };         // ACTIONS
  enum { MODE_NULL, MODE_CALLBACK, MODE_MACHINE, MODE_TMACHINE, MODE_FACTORY };

  Atm_multiplier& begin();
  Atm_multiplier& trace( Stream& stream );
  Atm_multiplier& onInput( multicb_t callback );
  Atm_multiplier& onInput( Machine& machine, state_t event = 0 );
  Atm_multiplier& onInput( const char* label, state_t event = 0 );
  Atm_multiplier& onInput( TinyMachine& machine, state_t event = 0 );

 private:
  int event( int id );
  void action( int id );
  void comm( Atm_Multiplier_Comm& c );
};

#endif
