#pragma once

#include <Automaton.h>

#define ATM_STEP_MAX 10

class Atm_step : public Machine {
 public:
  // clang-format off
  enum { LINEAR, S0, S1, S2, S3, S4, S5, S6, S7, S8, S9, R0, R1, R2, R3, R4, R5, R6, R7, R8, R9,
         SWEEP, X0, X1, X2, X3, X4, X5, X6, X7, X8, X9, XA, XB, XC, XD, XE, XF, XG, XH }; 
  enum { EVT_STEP, EVT_BACK, EVT_SWEEP, EVT_LINEAR, ELSE };
  // clang-format on

  Atm_step( void ) : Machine(){};
  Atm_step& begin( void );
  int state( void );
  Atm_step& trace( Stream& stream );
  Atm_step& onStep( uint8_t idx );  // fix id -> step???
  Atm_step& onStep( uint8_t id, atm_cb_push_t callback, int idx = 0 );
  Atm_step& onStep( uint8_t id, Machine& machine, int event = 0 );
  Atm_step& onStep( atm_cb_push_t callback, int idx = 0 );
  Atm_step& onStep( Machine& machine, int event = 0 );

 private:
  enum { ENT_S0, ENT_S1, ENT_S2, ENT_S3, ENT_S4, ENT_S5, ENT_S6, ENT_S7, ENT_S8, ENT_S9 };
  atm_connector connector[ATM_STEP_MAX];
  atm_connector onstep;
  int event( int id );
  void action( int id );
};
