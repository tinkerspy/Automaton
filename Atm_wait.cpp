#include <Automaton.h>
#include "Atm_wait.h"

Atm_wait & Atm_wait::begin( uint32_t period )
{
  const static state_t state_table[] PROGMEM = {
  /*            ON_ENTER    ON_LOOP  ON_EXIT  EVT_STOP  EVT_TIMER   ELSE */
  /* IDLE */          -1,        -1,      -1,       -1,        -1,    -1,
  /* WAIT */          -1,        -1,      -1,     IDLE,      IDLE,    -1 };
  Machine::begin( state_table, ELSE );
  timer.set( period );
  next = WAIT;
  return *this;
}

int Atm_wait::event( int id )
{
  return id == EVT_TIMER ? timer.expired( this ) : 0;
}

void Atm_wait::action( int id ) { }

Atm_wait & Atm_wait::onSwitch( swcb_sym_t switch_callback ) {

  Machine::onSwitch( switch_callback, "IDLE\0WAIT", "EVT_STOP\0EVT_TIMER\0ELSE" );
  return *this;
}

// Tiny Machine version

Att_wait & Att_wait::begin( uint32_t period )
{
  const static tiny_state_t state_table[] PROGMEM = {
  /*            ON_ENTER    ON_LOOP  ON_EXIT  EVT_STOP  EVT_TIMER   ELSE */
  /* IDLE */          -1,        -1,      -1,       -1,        -1,    -1,
  /* WAIT */          -1,        -1,      -1,     IDLE,      IDLE,    -1 };
  TinyMachine::begin( state_table, ELSE );
  timer.set( period );
  next = WAIT;
  return *this;
}

int Att_wait::event( int id )
{
  return id == EVT_TIMER ? timer.expired( this ) : 0;
}

void Att_wait::action( int id ) { }


