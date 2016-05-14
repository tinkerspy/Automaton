#pragma once

#include <Automaton.h>

#define TINYMACHINE
#undef FACTORY
#undef STATE_TYPE
#undef MACHINE

#ifdef TINYMACHINE
#define MACHINE TinyMachine
#define FACTORY 0
#define STATE_TYPE tiny_state_t
#else
#define MACHINE Machine
#define FACTORY factory
#define STATE_TYPE state_t
#endif

#define ATM_STEP_MAX 10

typedef bool ( *atm_step_cb_t )( int idx, int id );

class Att_step : public MACHINE {
 public:
  Att_step( void ) : MACHINE() {
#ifndef TINYMACHINE      
    class_label = "STP";
#endif    
  };
  // clang-format off
  enum { LINEAR,S0,S1,S2, S3, S4, S5, S6, S7, R0, R1, R2, R3, R4, R5, R6, R7, 
         SWEEP, X0, X1, X2, X3, X4, X5, X6, X7, X8, X9, XA, XB, XC, XD, 
         BURST, U0, U1, U2, U3, U4, U5, U6, U7 };
  enum { EVT_STEP, EVT_BACK, EVT_SWEEP, EVT_BURST, EVT_LINEAR, ELSE };
  enum { ACT_S0, ACT_S1, ACT_S2, ACT_S3, ACT_S4, ACT_S5, ACT_S6, ACT_S7 };
  // clang-format on
  
  atm_connector _connector[ATM_STEP_MAX];

  Att_step& begin( void );
  Att_step& trace( Stream& stream );
  int event( int id );
  void action( int id );
  Att_step& onStep( uint8_t idx );  
  Att_step& onStep( uint8_t id, atm_step_cb_t callback, int idx = 0 );
  Att_step& onStep( uint8_t id, Machine& machine, state_t event = 0 );
  Att_step& onStep( uint8_t id, const char* label, state_t event = 0 );
  Att_step& onStep( uint8_t idx, TinyMachine& machine, state_t event = 0 );

  
};

