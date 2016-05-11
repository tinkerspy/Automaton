#include "Att_step.hpp"

Att_step& Att_step::begin( void ) {
  // clang-format off
  const static STATE_TYPE state_table[] PROGMEM = { // TODO: Expand to 10!
    /*            ON_ENTER  ON_LOOP  ON_EXIT  EVT_STEP  EVT_BACK  EVT_SWEEP  EVT_BURST  EVT_LINEAR   ELSE */
    /* LINEAR  */       -1,      -1,      -1,       S0,       R7,     SWEEP,     BURST,     LINEAR,    -1,
    /* S0      */   ACT_S0,      -1,      -1,       S1,       R0,     SWEEP,     BURST,     LINEAR,    -1, // Linear mode
    /* S1      */   ACT_S1,      -1,      -1,       S2,       R1,     SWEEP,     BURST,     LINEAR,    -1,
    /* S2      */   ACT_S2,      -1,      -1,       S3,       R2,     SWEEP,     BURST,     LINEAR,    -1,
    /* S3      */   ACT_S3,      -1,      -1,       S4,       R3,     SWEEP,     BURST,     LINEAR,    -1,
    /* S4      */   ACT_S4,      -1,      -1,       S5,       R4,     SWEEP,     BURST,     LINEAR,    -1,
    /* S5      */   ACT_S5,      -1,      -1,       S6,       R5,     SWEEP,     BURST,     LINEAR,    -1,
    /* S6      */   ACT_S6,      -1,      -1,       S7,       R6,     SWEEP,     BURST,     LINEAR,    -1,
    /* S7      */   ACT_S7,      -1,      -1,       S0,       R7,     SWEEP,     BURST,     LINEAR,    -1,
    /* R0      */   ACT_S0,      -1,      -1,       S0,       R7,     SWEEP,     BURST,     LINEAR,    -1, // Linear mode R
    /* R1      */   ACT_S1,      -1,      -1,       S1,       R0,     SWEEP,     BURST,     LINEAR,    -1,
    /* R2      */   ACT_S2,      -1,      -1,       S2,       R1,     SWEEP,     BURST,     LINEAR,    -1,
    /* R3      */   ACT_S3,      -1,      -1,       S3,       R2,     SWEEP,     BURST,     LINEAR,    -1,
    /* R4      */   ACT_S4,      -1,      -1,       S4,       R3,     SWEEP,     BURST,     LINEAR,    -1,
    /* R5      */   ACT_S5,      -1,      -1,       S5,       R4,     SWEEP,     BURST,     LINEAR,    -1,
    /* R6      */   ACT_S6,      -1,      -1,       S6,       R5,     SWEEP,     BURST,     LINEAR,    -1,
    /* R7      */   ACT_S7,      -1,      -1,       S7,       R6,     SWEEP,     BURST,     LINEAR,    -1,
    /* SWEEP   */       -1,      -1,      -1,       X0,       X0,     SWEEP,     BURST,     LINEAR,    -1,
    /* X0      */   ACT_S0,      -1,      -1,       X1,       X1,     SWEEP,     BURST,     LINEAR,    -1, // Sweep mode
    /* X1      */   ACT_S1,      -1,      -1,       X2,       X2,     SWEEP,     BURST,     LINEAR,    -1,
    /* X2      */   ACT_S2,      -1,      -1,       X3,       X3,     SWEEP,     BURST,     LINEAR,    -1,
    /* X3      */   ACT_S3,      -1,      -1,       X4,       X4,     SWEEP,     BURST,     LINEAR,    -1,
    /* X4      */   ACT_S4,      -1,      -1,       X5,       X5,     SWEEP,     BURST,     LINEAR,    -1,
    /* X5      */   ACT_S5,      -1,      -1,       X6,       X6,     SWEEP,     BURST,     LINEAR,    -1,
    /* X6      */   ACT_S6,      -1,      -1,       X7,       X7,     SWEEP,     BURST,     LINEAR,    -1,
    /* X7      */   ACT_S7,      -1,      -1,       X8,       X8,     SWEEP,     BURST,     LINEAR,    -1,
    /* X8      */   ACT_S6,      -1,      -1,       X9,       X9,     SWEEP,     BURST,     LINEAR,    -1,
    /* X9      */   ACT_S5,      -1,      -1,       XA,       XA,     SWEEP,     BURST,     LINEAR,    -1,
    /* XA      */   ACT_S4,      -1,      -1,       XB,       XB,     SWEEP,     BURST,     LINEAR,    -1,
    /* XB      */   ACT_S3,      -1,      -1,       XC,       XC,     SWEEP,     BURST,     LINEAR,    -1,
    /* XC      */   ACT_S2,      -1,      -1,       XD,       XD,     SWEEP,     BURST,     LINEAR,    -1,
    /* XD      */   ACT_S1,      -1,      -1,       X0,       X0,     SWEEP,     BURST,     LINEAR,    -1,
    /* BURST   */       -1,      -1,      -1,       U0,       U0,     SWEEP,     BURST,     LINEAR,    -1, // Burst mode	
    /* U0      */   ACT_S0,      -1,      -1,       -1,       -1,     SWEEP,     BURST,     LINEAR,    U1, 
    /* U1      */   ACT_S1,      -1,      -1,       -1,       -1,     SWEEP,     BURST,     LINEAR,    U2,
    /* U2      */   ACT_S2,      -1,      -1,       -1,       -1,     SWEEP,     BURST,     LINEAR,    U3,
    /* U3      */   ACT_S3,      -1,      -1,       -1,       -1,     SWEEP,     BURST,     LINEAR,    U4,
    /* U4      */   ACT_S4,      -1,      -1,       -1,       -1,     SWEEP,     BURST,     LINEAR,    U5,
    /* U5      */   ACT_S5,      -1,      -1,       -1,       -1,     SWEEP,     BURST,     LINEAR,    U6,
    /* U6      */   ACT_S6,      -1,      -1,       -1,       -1,     SWEEP,     BURST,     LINEAR,    U7,
    /* U7      */   ACT_S7,      -1,      -1,       -1,       -1,     SWEEP,     BURST,     LINEAR, BURST,
  };
  // clang-format on
  MACHINE::begin( state_table, ELSE );
  return *this;
}

Att_step& Att_step::onStep( uint8_t id ) {
  _connector[id].mode_flags = atm_connector::MODE_NULL;
  return *this;
}

Att_step& Att_step::onStep( uint8_t id, atm_step_cb_t callback, int idx /* = 0 */ ) {
  _connector[id].set( (atm_cb_t)callback, idx );
  return *this;
}

Att_step& Att_step::onStep( uint8_t id, Machine& machine, state_t event /* = 0 */ ) {
  _connector[id].set( &machine, event );
  return *this;
}

Att_step& Att_step::onStep( uint8_t id, const char* label, state_t event /* = 0 */ ) {
  _connector[id].set( label, event );
  return *this;
}

Att_step& Att_step::onStep( uint8_t id, TinyMachine& machine, state_t event /* = 0 */ ) {
  _connector[id].set( &machine, event );
  return *this;
}

int Att_step::event( int id ) {
  state_t on_enter = read_state( state_table + ( current * state_width ) + ATM_ON_ENTER );
  switch ( id ) {
    case EVT_STEP:
      return ( current < R0 || current > R7 ) && ( on_enter > -1 ) && ( _connector[on_enter].mode() == 0 );
    case EVT_BACK:
      return ( current >= R0 && current <= R7 ) && ( on_enter > -1 ) && ( _connector[on_enter].mode() == 0 );
  }
  return 0;
}

void Att_step::action( int id ) {
  if ( id > -1 ) {
    if ( !_connector[id].push( FACTORY, true ) ) {
      ( *(atm_step_cb_t)_connector[id].callback )( _connector[id].callback_idx, id );
    }
  }
}

#ifndef TINYMACHINE
Att_step& Att_step::trace( Stream& stream ) {
  setTrace( &stream, atm_serial_debug::trace,
            "EVT_STEP\0EVT_BACK\0EVT_SWEEP\0EVT_BURST\0EVT_LINEAR\0ELSE\0"
            "LINEAR\0S0\0S1\0S2\0S3\0S4\0S5\0S6\0S7\0R0\0R1\0R2\0R3\0R4\0R5\0R6\0R7\0"
            "SWEEP\0X0\0X1\0X2\0X3\0X4\0X5\0X6\0X7\0X8\0X9\0XA\0XB\0XC\0XD\0"
            "BURST\0U0\0U1\0U2\0U3\0U4\0U5\0U6\0U7" );
  return *this;
}
#endif
