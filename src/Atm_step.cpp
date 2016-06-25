#include "Atm_step.hpp"

Atm_step& Atm_step::begin( void ) {
  // clang-format off
  const static state_t state_table[] PROGMEM = { 
    /*            ON_ENTER  ON_LOOP  ON_EXIT  EVT_STEP  EVT_BACK  EVT_SWEEP  EVT_LINEAR   ELSE */
    /* LINEAR  */       -1,      -1,      -1,       S0,       R9,     SWEEP,     LINEAR,    -1,
    /* S0      */   ENT_S0,      -1,      -1,       S1,       R0,     SWEEP,     LINEAR,    -1, // Linear
    /* S1      */   ENT_S1,      -1,      -1,       S2,       R1,     SWEEP,     LINEAR,    -1,
    /* S2      */   ENT_S2,      -1,      -1,       S3,       R2,     SWEEP,     LINEAR,    -1,
    /* S3      */   ENT_S3,      -1,      -1,       S4,       R3,     SWEEP,     LINEAR,    -1,
    /* S4      */   ENT_S4,      -1,      -1,       S5,       R4,     SWEEP,     LINEAR,    -1,
    /* S5      */   ENT_S5,      -1,      -1,       S6,       R5,     SWEEP,     LINEAR,    -1,
    /* S6      */   ENT_S6,      -1,      -1,       S7,       R6,     SWEEP,     LINEAR,    -1,
    /* S7      */   ENT_S7,      -1,      -1,       S8,       R7,     SWEEP,     LINEAR,    -1,
    /* S8      */   ENT_S8,      -1,      -1,       S9,       R8,     SWEEP,     LINEAR,    -1,
    /* S9      */   ENT_S9,      -1,      -1,       S0,       R9,     SWEEP,     LINEAR,    -1,
    /* R0      */   ENT_S0,      -1,      -1,       S0,       R9,     SWEEP,     LINEAR,    -1, // Linear R
    /* R1      */   ENT_S1,      -1,      -1,       S1,       R0,     SWEEP,     LINEAR,    -1,
    /* R2      */   ENT_S2,      -1,      -1,       S2,       R1,     SWEEP,     LINEAR,    -1,
    /* R3      */   ENT_S3,      -1,      -1,       S3,       R2,     SWEEP,     LINEAR,    -1,
    /* R4      */   ENT_S4,      -1,      -1,       S4,       R3,     SWEEP,     LINEAR,    -1,
    /* R5      */   ENT_S5,      -1,      -1,       S5,       R4,     SWEEP,     LINEAR,    -1,
    /* R6      */   ENT_S6,      -1,      -1,       S6,       R5,     SWEEP,     LINEAR,    -1,
    /* R7      */   ENT_S7,      -1,      -1,       S7,       R6,     SWEEP,     LINEAR,    -1,    
    /* R8      */   ENT_S8,      -1,      -1,       S8,       R7,     SWEEP,     LINEAR,    -1,
    /* R9      */   ENT_S9,      -1,      -1,       S9,       R8,     SWEEP,     LINEAR,    -1,
    /* SWEEP   */       -1,      -1,      -1,       X0,       X0,     SWEEP,     LINEAR,    -1,
    /* X0      */   ENT_S0,      -1,      -1,       X1,       X1,     SWEEP,     LINEAR,    -1, // Sweep
    /* X1      */   ENT_S1,      -1,      -1,       X2,       X2,     SWEEP,     LINEAR,    -1,
    /* X2      */   ENT_S2,      -1,      -1,       X3,       X3,     SWEEP,     LINEAR,    -1,
    /* X3      */   ENT_S3,      -1,      -1,       X4,       X4,     SWEEP,     LINEAR,    -1,
    /* X4      */   ENT_S4,      -1,      -1,       X5,       X5,     SWEEP,     LINEAR,    -1,
    /* X5      */   ENT_S5,      -1,      -1,       X6,       X6,     SWEEP,     LINEAR,    -1,
    /* X6      */   ENT_S6,      -1,      -1,       X7,       X7,     SWEEP,     LINEAR,    -1,
    /* X7      */   ENT_S7,      -1,      -1,       X8,       X8,     SWEEP,     LINEAR,    -1,
    /* X8      */   ENT_S8,      -1,      -1,       X9,       X9,     SWEEP,     LINEAR,    -1,
    /* X9      */   ENT_S9,      -1,      -1,       XA,       XA,     SWEEP,     LINEAR,    -1,
    /* XA      */   ENT_S8,      -1,      -1,       XB,       XB,     SWEEP,     LINEAR,    -1,
    /* XB      */   ENT_S7,      -1,      -1,       XC,       XC,     SWEEP,     LINEAR,    -1,
    /* XC      */   ENT_S6,      -1,      -1,       XD,       XD,     SWEEP,     LINEAR,    -1,
    /* XD      */   ENT_S5,      -1,      -1,       XE,       XE,     SWEEP,     LINEAR,    -1,
    /* XE      */   ENT_S4,      -1,      -1,       XF,       XF,     SWEEP,     LINEAR,    -1,
    /* XF      */   ENT_S3,      -1,      -1,       XG,       XG,     SWEEP,     LINEAR,    -1,
    /* XG      */   ENT_S2,      -1,      -1,       XH,       XH,     SWEEP,     LINEAR,    -1,
    /* XH      */   ENT_S1,      -1,      -1,       X0,       X0,     SWEEP,     LINEAR,    -1,
  };
  // clang-format on
  Machine::begin( state_table, ELSE );
  return *this;
}

int Atm_step::event( int id ) {
  int on_enter = read_state( state_table + ( current * state_width ) + ATM_ON_ENTER );
  switch ( id ) {
    case EVT_STEP:
      return ( current < R0 || current > R9 ) && ( on_enter > -1 ) && ( connector[on_enter].mode() == 0 );
    case EVT_BACK:
      return ( current >= R0 && current <= R9 ) && ( on_enter > -1 ) && ( connector[on_enter].mode() == 0 );
  }
  return 0;
}

void Atm_step::action( int id ) {
  if ( id > -1 ) {
    if ( connector[id].mode() ) {
      connector[id].push( id );
      onstep.push( id );
    }
  }
}

Atm_step& Atm_step::onStep( uint8_t id ) {
  connector[id].mode_flags = atm_connector::MODE_NULL;
  return *this;
}

Atm_step& Atm_step::onStep( uint8_t id, atm_cb_push_t callback, int idx /* = 0 */ ) {
  connector[id].set( callback, idx );
  return *this;
}

Atm_step& Atm_step::onStep( uint8_t id, Machine& machine, int event /* = 0 */ ) {
  connector[id].set( &machine, event );
  return *this;
}

Atm_step& Atm_step::onStep( atm_cb_push_t callback, int idx /* = 0 */ ) {
  onstep.set( callback, idx );
  return *this;
}

Atm_step& Atm_step::onStep( Machine& machine, int event /* = 0 */ ) {
  onstep.set( &machine, event );
  return *this;
}

int Atm_step::state( void ) {
  int on_enter = read_state( state_table + ( current * state_width ) + ATM_ON_ENTER );
  return on_enter;
}

Atm_step& Atm_step::trace( Stream& stream ) {
  setTrace( &stream, atm_serial_debug::trace,
            "STEP\0EVT_STEP\0EVT_BACK\0EVT_SWEEP\0EVT_LINEAR\0ELSE\0"
            "LINEAR\0S0\0S1\0S2\0S3\0S4\0S5\0S6\0S7\0S8\0S9\0R0\0R1\0R2\0R3\0R4\0R5\0R6\0R7\0R8\0R9\0"
            "SWEEP\0X0\0X1\0X2\0X3\0X4\0X5\0X6\0X7\0X8\0X9\0XA\0XB\0XC\0XD\0XE\0XF\0XG\0XH" );
  return *this;
}
