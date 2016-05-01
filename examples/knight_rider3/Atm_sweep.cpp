#include <Automaton.h>
#include "Atm_sweep.h"

Atm_sweep & Atm_sweep::begin( int p0, int p1, int p2, int p3, int p4, int p5 )
{
  const static state_t state_table[] PROGMEM = {
    /*          ON_ENTER    ON_LOOP  ON_EXIT  EVT_TIMER   EVT_START  EVT_STOP  EVT_TOGGLE, ELSE */
    /* IDLE  */  ACT_OFF, ATM_SLEEP,      -1,        -1,         U0,     IDLE,         U0,   -1,
    /* U0    */   ACT_L0,        -1, ACT_OFF,        U1,         -1,     IDLE,       IDLE,   -1,
    /* U1    */   ACT_L1,        -1, ACT_OFF,        U2,         -1,     IDLE,       IDLE,   -1,
    /* U2    */   ACT_L2,        -1, ACT_OFF,        U3,         -1,     IDLE,       IDLE,   -1,
    /* U3    */   ACT_L3,        -1, ACT_OFF,        U4,         -1,     IDLE,       IDLE,   -1,
    /* U4    */   ACT_L4,        -1, ACT_OFF,        U5,         -1,     IDLE,       IDLE,   -1,
    /* U5    */   ACT_L5,        -1, ACT_OFF,        D4,         -1,     IDLE,       IDLE,   -1,
    /* D4    */   ACT_L4,        -1, ACT_OFF,        D3,         -1,     IDLE,       IDLE,   -1,
    /* D3    */   ACT_L3,        -1, ACT_OFF,        D2,         -1,     IDLE,       IDLE,   -1,
    /* D2    */   ACT_L2,        -1, ACT_OFF,        D1,         -1,     IDLE,       IDLE,   -1,
    /* D1    */   ACT_L1,        -1, ACT_OFF,        D0,         -1,     IDLE,       IDLE,   -1,
    /* D0    */   ACT_L0,        -1, ACT_OFF,        U0,         -1,     IDLE,       IDLE,   -1,
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

Atm_sweep & Atm_sweep::speed( uint32_t v )
{
  timer.set( v );
  return *this;
}


int Atm_sweep::event( int id )
{
  switch ( id ) {
    case EVT_TIMER :
      return timer.expired( this );
  }
  return 0;
}

void Atm_sweep::action( int id )
{
  switch ( id ) {
    case ACT_L0 : return digitalWrite( pin[0], HIGH );
    case ACT_L1 : return digitalWrite( pin[1], HIGH );
    case ACT_L2 : return digitalWrite( pin[2], HIGH );
    case ACT_L3 : return digitalWrite( pin[3], HIGH );
    case ACT_L4 : return digitalWrite( pin[4], HIGH );
    case ACT_L5 : return digitalWrite( pin[5], HIGH );
    case ACT_OFF :
      for ( uint8_t i = 0; i <= 5; i++ ) {
        digitalWrite( pin[i], LOW );
      }
      return;
  }
}

Atm_sweep & Atm_sweep::trace( Stream & stream ) {

  Machine::setTrace( &stream, atm_serial_debug::trace,
       "EVT_TIMER\0EVT_START\0EVT_STOP\0ELSE\0"
       "IDLE\0U0\0U1\0U2\0U3\0U4\0U5\0D4\0D3\0D2\0D1\0D0" );
  return *this;
}

