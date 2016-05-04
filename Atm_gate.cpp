#include "Atm_gate.h"

Atm_gate & Atm_gate::begin( int numberOfInputs )
{
  const static state_t state_table[] PROGMEM = {
    /*             ON_ENTER    ON_LOOP  ON_EXIT  EVT_G0 EVT_G1 EVT_G2 EVT_G3 EVT_G4 EVT_G5 EVT_G6 EVT_G7 EVT_OPEN  EVT_CLEAR   ELSE */
    /* IDLE    */        -1,        -1,      -1,     G0,    G1,    G2,    G3,    G4,    G5,    G6,    G7,    OPEN,     CLEAR,    -1,
    /* G0      */    ACT_G0,        -1,      -1,     -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,      -1,        -1, CHECK,
    /* G1      */    ACT_G1,        -1,      -1,     -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,      -1,        -1, CHECK,
    /* G2      */    ACT_G2,        -1,      -1,     -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,      -1,        -1, CHECK,
    /* G3      */    ACT_G3,        -1,      -1,     -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,      -1,        -1, CHECK,
    /* G4      */    ACT_G4,        -1,      -1,     -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,      -1,        -1, CHECK,
    /* G5      */    ACT_G5,        -1,      -1,     -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,      -1,        -1, CHECK,
    /* G6      */    ACT_G6,        -1,      -1,     -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,      -1,        -1, CHECK,
    /* G7      */    ACT_G7,        -1,      -1,     -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,      -1,        -1, CHECK,
    /* CHECK   */        -1,        -1,      -1,     -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    OPEN,        -1,  IDLE,
    /* OPEN    */  ACT_OPEN,        -1,      -1,     -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,      -1,        -1, CLEAR,
    /* CLEAR   */ ACT_CLEAR,        -1,      -1,     -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,      -1,        -1,  IDLE,
  };
  Machine::begin( state_table, ELSE );
  _gate_count = numberOfInputs;
  gates_init( _gate_count );
  return *this;
}

Atm_gate & Atm_gate::onOpen( opencb_t callback, int idx /* = 0 */ )
{
  _callback = callback;
  _callback_idx = idx;
  _callback_count = 0;
  flags &= ~ATM_USR_FLAGS;
  flags |= ATM_USR1_FLAG;
  return *this;
}

Atm_gate & Atm_gate::onOpen( Machine & machine, int event /* = 0 */ )
{
  _client_machine = &machine;
  _client_machine_event = event;
  flags &= ~ATM_USR_FLAGS;
  flags |= ATM_USR2_FLAG;
  return *this;
}

Atm_gate & Atm_gate::onOpen( const char * label, int event /* = 0 */ )
{
  _client_label = label;
  _client_label_event = event;
  flags &= ~ATM_USR_FLAGS;
  flags |= ATM_USR3_FLAG;
  return *this;
}

Atm_gate & Atm_gate::onOpen( TinyMachine & machine, int event /* = 0 */ )
{
  _client_tmachine = &machine;
  _client_tmachine_event = event;
  flags &= ~ATM_USR_FLAGS;
  flags |= ATM_USR4_FLAG;
  return *this;
}

void Atm_gate::gates_init( int count )
{
  _gates = 0;
  for ( short i = 0; i < count; i ++ ) {
    _gates |= ( 1 << i );
  }
}

void Atm_gate::gate_clear( int id )
{
  _gates &= ~( 1 << id );
}

int Atm_gate::event( int id )
{
  switch ( id ) {
    case EVT_OPEN:
      return _gates == 0;
  }
  return 0;
}

void Atm_gate::action( int id )
{
  switch ( id ) {
    case ACT_G0: return gate_clear( 0 );
    case ACT_G1: return gate_clear( 1 );
    case ACT_G2: return gate_clear( 2 );
    case ACT_G3: return gate_clear( 3 );
    case ACT_G4: return gate_clear( 4 );
    case ACT_G5: return gate_clear( 5 );
    case ACT_G6: return gate_clear( 6 );
    case ACT_G7: return gate_clear( 7 );
    case ACT_CLEAR: return gates_init( _gate_count );
    case ACT_OPEN:
      if ( ( flags & ATM_USR1_FLAG ) > 0 ) {
        _callback_count++;
        (*_callback)( _callback_idx, _callback_count );
      }
      if ( ( flags & ATM_USR2_FLAG ) > 0 ) {
        _client_machine->trigger( _client_machine_event );
      }
      if ( ( flags & ATM_USR3_FLAG ) > 0 && factory ) {
        factory->trigger( _client_label, _client_label_event );
      }
      if ( ( flags & ATM_USR4_FLAG ) > 0 ) {
        _client_tmachine->trigger( _client_tmachine_event );
      }
      return;
  }
}

Atm_gate & Atm_gate::trace( Stream & stream )
{
  setTrace( &stream, atm_serial_debug::trace,
            "EVT_G0\0EVT_G1\0EVT_G2\0EVT_G3\0EVT_G4\0EVT_G5\0EVT_G6\0EVT_G7\0EVT_OPEN\0EVT_CLEAR\0ELSE\0"
            "IDLE\0G0\0G1\0G2\0G3\0G4\0G5\0G6\0G7\0CHECK\0OPEN\0CLEAR" );
  return *this;
}

// Tiny Machine version


Att_gate & Att_gate::begin( int numberOfInputs )
{
  const static tiny_state_t state_table[] PROGMEM = {
    /*             ON_ENTER    ON_LOOP  ON_EXIT  EVT_G0 EVT_G1 EVT_G2 EVT_G3 EVT_G4 EVT_G5 EVT_G6 EVT_G7 EVT_OPEN  EVT_CLEAR   ELSE */
    /* IDLE    */        -1,        -1,      -1,     G0,    G1,    G2,    G3,    G4,    G5,    G6,    G7,    OPEN,     CLEAR,    -1,
    /* G0      */    ACT_G0,        -1,      -1,     -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,      -1,        -1, CHECK,
    /* G1      */    ACT_G1,        -1,      -1,     -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,      -1,        -1, CHECK,
    /* G2      */    ACT_G2,        -1,      -1,     -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,      -1,        -1, CHECK,
    /* G3      */    ACT_G3,        -1,      -1,     -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,      -1,        -1, CHECK,
    /* G4      */    ACT_G4,        -1,      -1,     -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,      -1,        -1, CHECK,
    /* G5      */    ACT_G5,        -1,      -1,     -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,      -1,        -1, CHECK,
    /* G6      */    ACT_G6,        -1,      -1,     -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,      -1,        -1, CHECK,
    /* G7      */    ACT_G7,        -1,      -1,     -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,      -1,        -1, CHECK,
    /* CHECK   */        -1,        -1,      -1,     -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    OPEN,        -1,  IDLE,
    /* OPEN    */  ACT_OPEN,        -1,      -1,     -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,      -1,        -1, CLEAR,
    /* CLEAR   */ ACT_CLEAR,        -1,      -1,     -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,      -1,        -1,  IDLE,
  };
  TinyMachine::begin( state_table, ELSE );
  _gate_count = numberOfInputs;
  gates_init( _gate_count );
  return *this;
}

Att_gate & Att_gate::onOpen( opencb_t callback, int idx /* = 0 */ )
{
  _callback = callback;
  _callback_idx = idx;
  _callback_count = 0;
  flags &= ~ATM_USR_FLAGS;
  flags |= ATM_USR1_FLAG;
  return *this;
}

Att_gate & Att_gate::onOpen( Machine & machine, int event /* = 0 */ )
{
  _client_machine = &machine;
  _client_machine_event = event;
  flags &= ~ATM_USR_FLAGS;
  flags |= ATM_USR2_FLAG;
  return *this;
}

Att_gate & Att_gate::onOpen( TinyMachine & machine, int event /* = 0 */ )
{
  _client_tmachine = &machine;
  _client_tmachine_event = event;
  flags &= ~ATM_USR_FLAGS;
  flags |= ATM_USR4_FLAG;
  return *this;
}

void Att_gate::gates_init( int count )
{
  _gates = 0;
  for ( short i = 0; i < count; i ++ ) {
    _gates |= ( 1 << i );
  }
}

void Att_gate::gate_clear( int id )
{
  _gates &= ~( 1 << id );
}

int Att_gate::event( int id )
{
  switch ( id ) {
    case EVT_OPEN:
      return _gates == 0;
  }
  return 0;
}

void Att_gate::action( int id )
{
  switch ( id ) {
    case ACT_G0: return gate_clear( 0 );
    case ACT_G1: return gate_clear( 1 );
    case ACT_G2: return gate_clear( 2 );
    case ACT_G3: return gate_clear( 3 );
    case ACT_G4: return gate_clear( 4 );
    case ACT_G5: return gate_clear( 5 );
    case ACT_G6: return gate_clear( 6 );
    case ACT_G7: return gate_clear( 7 );
    case ACT_CLEAR: return gates_init( _gate_count );
    case ACT_OPEN:
      if ( ( flags & ATM_USR1_FLAG ) > 0 ) {
        _callback_count++;
        (*_callback)( _callback_idx, _callback_count );
      }
      if ( ( flags & ATM_USR2_FLAG ) > 0 ) {
        _client_machine->trigger( _client_machine_event );
      }
      if ( ( flags & ATM_USR4_FLAG ) > 0 ) {
        _client_tmachine->trigger( _client_tmachine_event );
      }
      return;
  }
}

