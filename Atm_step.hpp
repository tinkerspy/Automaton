
#ifndef Atm_step_h
#define Atm_step_h

#include <Automaton.h>

typedef void ( *stepcb_t )( int idx );

class Step {
 public:
  uint8_t _mode = 0;
  union {
    struct {  // ATM_USR1_FLAG - callback
      void ( *_callback )( int idx );
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

class Atm_step : public Machine {
 public:
  Atm_step( void ) : Machine() {
    class_label = "STP";
  };
  enum {
    LINEAR,
    S0,
    S1,
    S2,
    S3,
    S4,
    S5,
    S6,
    S7,
    R0,
    R1,
    R2,
    R3,
    R4,
    R5,
    R6,
    R7,
    SWEEP,
    X0,
    X1,
    X2,
    X3,
    X4,
    X5,
    X6,
    X7,
    X8,
    X9,
    XA,
    XB,
    XC,
    XD,
    BURST,
    U0,
    U1,
    U2,
    U3,
    U4,
    U5,
    U6,
    U7
  };
  enum { EVT_STEP, EVT_BACK, EVT_SWEEP, EVT_BURST, EVT_LINEAR, ELSE };
  enum { ACT_S0, ACT_S1, ACT_S2, ACT_S3, ACT_S4, ACT_S5, ACT_S6, ACT_S7 };
  enum { MODE_NULL, MODE_CALLBACK, MODE_MACHINE, MODE_FACTORY, MODE_TMACHINE };
  Step _step[8];

  Atm_step& begin( void );
  Atm_step& trace( Stream& stream );
  int event( int id );
  void action( int id );
  Atm_step& onStep( uint8_t idx );
  Atm_step& onStep( uint8_t idx, stepcb_t callback );
  Atm_step& onStep( uint8_t idx, Machine& machine, state_t event = 0 );
  Atm_step& onStep( uint8_t idx, TinyMachine& machine, state_t event = 0 );
  Atm_step& onStep( uint8_t idx, const char* label, state_t event = 0 );
};


#endif
