#include "Atm_teensywave.h"

// TODO
// - Triangle waveform
// - Combine with LCD interface

Atm_teensywave & Atm_teensywave::begin( int attached_pin, int steps, int delay )
{
      const static state_t state_table[] PROGMEM = {
      /*                ON_ENTER    ON_LOOP  ON_EXIT  EVT_COUNTER   EVT_TIMER  EVT_TOGGLE   ELSE */
      /* IDLE     */    ACT_IDLE, ATM_SLEEP,      -1,          -1,         -1,   START_SN,    -1,
      /* START_SN */   ACT_START,        -1,      -1,          -1,         -1,         -1,  SINE, // SINE
      /* SINE     */    ACT_SINE,        -1,      -1,    START_SN,       SINE,   START_SW,    -1,
      /* START_SW */   ACT_START,        -1,      -1,          -1,         -1,         -1,   SAW,
      /* SAW      */     ACT_SAW,        -1,      -1,    START_SW,        SAW,   START_SR,    -1, // SAWTOOTH
      /* START_SR */   ACT_START,        -1,      -1,          -1,         -1,         -1,  SAWR,
      /* SAWR     */    ACT_SAWR,        -1,      -1,    START_SR,       SAWR,   START_TR,    -1, // SAWTOOTH REVERSED
      /* START_TR */ ACT_STARTTR,        -1,      -1,          -1,         -1,         -1,   TRI,
      /* TRI      */     ACT_TRI,        -1,      -1,    START_TR,        TRI,   START_SQ,    -1, // TRIANGLE
      /* START_SQ */ ACT_STARTSQ,        -1,      -1,          -1,         -1,         -1,  SQON, // SQUARE
      /* SQON     */    ACT_SQON,        -1,      -1,          -1,      SQOFF,         -1,    -1,
      /* SQOFF    */   ACT_SQOFF,        -1,      -1,          -1,       SQON,   START_SN,    -1,
      };
      Machine::begin( state_table, ELSE );
      pin = attached_pin; 
      _steps = steps;
      _delay = delay;
      timer.set( delay ); 
      phase.set( steps ); // 314 steps of 0.02 = 6.28 ( 2 * pi )
      _stepsize = (float) 6.28318 / _steps;
      pinMode( pin, OUTPUT ); 
// Analog out is only available on Teensy 3.1 and LC      
#if defined(__MK20DX256__) || defined(__MKL26Z64__)
      analogWriteResolution( 12 ); 
#endif
      return *this;          
}

int Atm_teensywave::event( int id ) 
{
  switch ( id ) {
    case EVT_TIMER :
      return timer.expired( this );        
    case EVT_COUNTER :
      return phase.expired();        
   }
   return 0;
}

void Atm_teensywave::action( int id ) 
{
  switch ( id ) {
    case ACT_IDLE :
      analogWrite( pin, 0 );
      return;
    case ACT_START :
      timer.set( _delay ); 
      phase.set( _steps );
      return;
    case ACT_STARTSQ :
      timer.set( _steps * _delay / 2 ); 
      return;
    case ACT_SINE :        
      analogWrite( pin, sin( phase.value * _stepsize ) * 2000.0 + 2050.0 );
      phase.decrement();
      return;
    case ACT_SAW :        
      analogWrite( pin, ( _steps - phase.value ) * 13 );
      phase.decrement();
      return;
    case ACT_SAWR :        
      analogWrite( pin, phase.value * 13 );
      phase.decrement();
      return;
    case ACT_STARTTR :
      timer.set( _delay ); 
      phase.set( _steps );
      return;
    case ACT_TRI :        
      if ( ( phase.value << 1 ) >= _steps ) {
        analogWrite( pin, ( _steps - phase.value ) * 26 );
      } else {
        analogWrite( pin, phase.value * 26 );
      }
      phase.decrement();
      return;
    case ACT_SQON :        
      analogWrite( pin, 4095 );
      return;
    case ACT_SQOFF :        
      analogWrite( pin, 0 );
      return;
   }
}

Atm_teensywave & Atm_teensywave::trace( Stream * stream, swcb_sym_t switch_callback ) {

  Machine::trace( stream, switch_callback, 
  "IDLE\0START_SN\0SINE\0START_SW\0SAW\0START_SR\0SAWR"
  "\0START_TR\0TRI\0START_SQ\0SQON\0SQOFF",
  "EVT_COUNTER\0EVT_TIMER\0EVT_TOGGLE\0ELSE" );    
  return *this;
}

