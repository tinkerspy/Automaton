#pragma once

#include <Automaton.h>

#undef TINYMACHINE
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

class Atm_timer : public MACHINE {
 public:
  Atm_timer( void ) : MACHINE() {
#ifndef TINYMACHINE      
    class_label = "TMR";
#endif    
  };

  atm_timer_millis daytimer, mstimer;
  atm_counter daycounter, repcounter;
  uint16_t days;
  uint16_t repeat_cnt;
  atm_connector _ontimer;

  enum { IDLE, START, WAITD, WAITMS, TRIGGER };
  enum { EVT_DAYCNT, EVT_DAYTIMER, EVT_MSTIMER, EVT_REPCNT, EVT_OFF, EVT_ON, ELSE };
  enum { ACT_START, ACT_TRIG, ACT_WAITD };

  Atm_timer& begin( uint32_t ms = 0 );
  Atm_timer& trace( Stream& stream );
  Atm_timer& onTimer( atm_cb_t callback, int idx = 0 );
  Atm_timer& onTimer( Machine& machine, int event = 0 );
  Atm_timer& onTimer( TinyMachine& machine, int event = 0 );
  Atm_timer& onTimer( const char* label, int event = 0 );
  Atm_timer& interval_seconds( uint32_t v );
  Atm_timer& interval_millis( uint32_t v );
  Atm_timer& interval( uint32_t v );
  Atm_timer& repeat( uint16_t v );
  int event( int id );
  void action( int id );
};

