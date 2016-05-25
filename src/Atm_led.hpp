#pragma once

#include <Automaton.h>

class Atm_led : public Machine {
 public:
  enum { IDLE, ON, START, BLINK_OFF, DONE, OFF };
  enum { EVT_ON_TIMER, EVT_OFF_TIMER, EVT_COUNTER, EVT_ON, EVT_OFF, EVT_BLINK, EVT_TOGGLE, EVT_TOGGLE_BLINK, ELSE };

  Atm_led( void ) : Machine(){};
  Atm_led& begin( int attached_pin, bool activeLow = false );
  Atm_led& blink( uint32_t duration );
  Atm_led& blink( uint32_t duration, uint32_t pause_duration, uint16_t repeat_count = ATM_COUNTER_OFF );
  Atm_led& pause( uint32_t duration );
  Atm_led& fade( int fade );
  Atm_led& repeat( int repeat );
  Atm_led& brightness( uint8_t level );
  Atm_led& trace( Stream& stream );
  Atm_led& onFinish( Machine& machine, int event = 0 );
  Atm_led& onFinish( atm_cb_push_t callback, int idx = 0 );

 private:
  enum { ACT_INIT, ACT_ON, ACT_OFF, ACT_CHAIN };
  uint8_t level;
  short pin;
  bool activeLow;
  int repeat_count;
  atm_timer_millis on_timer, off_timer;
  atm_counter counter;
  atm_connector onfinish;

  int event( int id );
  void action( int id );
};
