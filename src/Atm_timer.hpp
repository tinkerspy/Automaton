#pragma once

#include <Automaton.h>

class Atm_timer : public Machine {
 public:
  enum { IDLE, START, WAITD, WAITMS, TRIGGER, FINISH };
  enum { EVT_DAYCNT, EVT_DAYTIMER, EVT_MSTIMER, EVT_REPCNT, EVT_STOP, EVT_START, EVT_TOGGLE, ELSE }; // EVT_PAUSE, EVT_RESUME

  Atm_timer( void ) : Machine(){};
  Atm_timer& begin( uint32_t ms = 0, uint16_t repeats = 1 );
  Atm_timer& trace( Stream& stream );
  Atm_timer& onTimer( atm_cb_push_t callback, int idx = 0 );
  Atm_timer& onTimer( Machine& machine, int event = 0 );
  Atm_timer& onFinish( atm_cb_push_t callback, int idx = 0 );
  Atm_timer& onFinish( Machine& machine, int event = 0 );
  Atm_timer& interval_seconds( uint32_t v );
  Atm_timer& interval_millis( uint32_t v );
  Atm_timer& interval( uint32_t v );
  uint32_t left();
  Atm_timer& repeat( uint16_t v = ATM_COUNTER_OFF );
  Atm_timer& start( void );
  Atm_timer& stop( void );
  Atm_timer& toggle( void );
//  Atm_timer& pause( void ); TODO
//  Atm_timer& resume( void );
  

 private:
  enum { ENT_START, ENT_TRIGGER, EXT_WAITD, ENT_FINISH };
  atm_timer_millis daytimer, mstimer;
  atm_counter daycounter, repcounter;
  uint32_t abscounter;
  uint16_t days;
  uint16_t repeat_cnt;
  atm_connector ontimer, onfinish;

  int event( int id );
  void action( int id );
};
