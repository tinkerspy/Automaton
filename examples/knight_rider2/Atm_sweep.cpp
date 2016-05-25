#include <Automaton.h>
#include "Atm_sweep.h"

Atm_sweep & Atm_sweep::begin( int p0, int p1, int p2, int p3, int p4, int p5 ) {
  const static state_t state_table[] PROGMEM = {
    /*          ON_ENTER    ON_LOOP  ON_EXIT  EVT_TIMER   EVT_START  EVT_STOP  EVT_TOGGLE, ELSE */
    /* IDLE  */  ENT_OFF, ATM_SLEEP,      -1,        -1,         U0,     IDLE,         U0,   -1,
    /* U0    */   ENT_L0,        -1, ENT_OFF,        U1,         -1,     IDLE,       IDLE,   -1,
    /* U1    */   ENT_L1,        -1, ENT_OFF,        U2,         -1,     IDLE,       IDLE,   -1,
    /* U2    */   ENT_L2,        -1, ENT_OFF,        U3,         -1,     IDLE,       IDLE,   -1,
    /* U3    */   ENT_L3,        -1, ENT_OFF,        U4,         -1,     IDLE,       IDLE,   -1,
    /* U4    */   ENT_L4,        -1, ENT_OFF,        U5,         -1,     IDLE,       IDLE,   -1,
    /* U5    */   ENT_L5,        -1, ENT_OFF,        D4,         -1,     IDLE,       IDLE,   -1,
    /* D4    */   ENT_L4,        -1, ENT_OFF,        D3,         -1,     IDLE,       IDLE,   -1,
    /* D3    */   ENT_L3,        -1, ENT_OFF,        D2,         -1,     IDLE,       IDLE,   -1,
    /* D2    */   ENT_L2,        -1, ENT_OFF,        D1,         -1,     IDLE,       IDLE,   -1,
    /* D1    */   ENT_L1,        -1, ENT_OFF,        D0,         -1,     IDLE,       IDLE,   -1,
    /* D0    */   ENT_L0,        -1, ENT_OFF,        U0,         -1,     IDLE,       IDLE,   -1,
  };
  Machine::begin( state_table, ELSE );
  pin[0] = p0;  pin[1] = p1;  pin[2] = p2;
  pin[3] = p3;  pin[4] = p4;  pin[5] = p5;
  timer.set( 200 );
  for ( uint8_t i = 0; i <= 5; i++ ) {
    pinMode( pin[i], OUTPUT );
  }
  return *this;
}

Atm_sweep & Atm_sweep::speed( uint32_t v ) {
  timer.set( v );
  return *this;
}

int Atm_sweep::event( int id ) {
  switch ( id ) {
    case EVT_TIMER :
      return timer.expired( this );
  }
  return 0;
}

void Atm_sweep::action( int id ) {
  switch ( id ) {
    case ENT_L0 : return digitalWrite( pin[0], HIGH );
    case ENT_L1 : return digitalWrite( pin[1], HIGH );
    case ENT_L2 : return digitalWrite( pin[2], HIGH );
    case ENT_L3 : return digitalWrite( pin[3], HIGH );
    case ENT_L4 : return digitalWrite( pin[4], HIGH );
    case ENT_L5 : return digitalWrite( pin[5], HIGH );
    case ENT_OFF :
      for ( uint8_t i = 0; i <= 5; i++ ) {
        digitalWrite( pin[i], LOW );
      }
      return;
  }
}

Atm_sweep & Atm_sweep::trace( Stream & stream ) {
  Machine::setTrace( &stream, atm_serial_debug::trace,
       "SWEEP\0EVT_TIMER\0EVT_START\0EVT_STOP\0ELSE\0"
       "IDLE\0U0\0U1\0U2\0U3\0U4\0U5\0D4\0D3\0D2\0D1\0D0" );
  return *this;
}

