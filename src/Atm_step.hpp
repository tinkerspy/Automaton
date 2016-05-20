#pragma once

#include <Automaton.h>

#define ATM_STEP_MAX 10

typedef bool ( *atm_step_cb_t )( int idx, int id );

class Atm_step : public Machine {
 public:
  Atm_step( void ) : Machine(){};
  // clang-format off
  enum { LINEAR, S0, S1, S2, S3, S4, S5, S6, S7, S8, S9, R0, R1, R2, R3, R4, R5, R6, R7, R8, R9,
         SWEEP, X0, X1, X2, X3, X4, X5, X6, X7, X8, X9, XA, XB, XC, XD, XE, XF, XG, XH }; 
  enum { EVT_STEP, EVT_BACK, EVT_SWEEP, EVT_LINEAR, ELSE };
  enum { ACT_S0, ACT_S1, ACT_S2, ACT_S3, ACT_S4, ACT_S5, ACT_S6, ACT_S7, ACT_S8, ACT_S9 };
  // clang-format on

  atm_connector _connector[ATM_STEP_MAX];

  Atm_step& begin( void );
  Atm_step& trace( Stream& stream );
  int event( int id );
  void action( int id );
  Atm_step& onStep( uint8_t idx );  // fix id -> step???
  Atm_step& onStep( uint8_t id, atm_step_cb_t callback, int idx = 0 );
  Atm_step& onStep( uint8_t id, Machine& machine, int event = 0 );
};
