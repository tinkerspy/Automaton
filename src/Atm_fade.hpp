#pragma once

#include <Automaton.h>

class Atm_fade : public Machine {
 public:
  enum { IDLE, ON, START, STARTU, UP, STARTD, DOWN, REPEAT, DONE, OSTARTU, OUP, OSTARTD, ODOWN };
  enum { EVT_CNT_FADE, EVT_TM_FADE, EVT_TM_ON, EVT_TM_OFF, EVT_CNT_RPT, EVT_ON, EVT_OFF, EVT_BLINK, EVT_TOGGLE, EVT_TOGGLE_BLINK, ELSE };
  enum { EVT_START = EVT_BLINK };

  Atm_fade( void ) : Machine(){};
  Atm_fade& begin( int attached_pin );
  Atm_fade& trace( Stream& stream );
  Atm_fade& blink( uint32_t duration, uint32_t pause_duration, uint16_t repeat_count = ATM_COUNTER_OFF );
  Atm_fade& blink( uint32_t duration );
  Atm_fade& blink( void );
  Atm_fade& pause( uint32_t duration );
  Atm_fade& fade( int fade );
  Atm_fade& repeat( uint16_t repeat );
  Atm_fade& on( void );
  Atm_fade& off( void );
  Atm_fade& toggle( void );
  Atm_fade& toggleBlink( void );
  Atm_fade& start( void );
  Atm_fade& onFinish( Machine& machine, int event = 0 );
  Atm_fade& onFinish( atm_cb_push_t callback, int idx = 0 );

 private:
  enum { ENT_REPEAT, ENT_OFF, ENT_ON, ENT_UP, ENT_DOWN, ENT_START, ENT_DONE };
  static const uint8_t SLOPE_SIZE = 32;
  uint8_t slope[SLOPE_SIZE] = {0, 0, 1, 1, 2, 2, 3, 4, 5, 6, 7, 8, 10, 12, 15, 18, 22, 26, 31, 37, 44, 54, 63, 76, 90, 108, 127, 153, 180, 217, 230, 255};
  short pin;
  uint16_t repeat_count;
  atm_connector onfinish;
  atm_timer_millis timer_fade, timer_on, timer_off;
  atm_counter counter_fade, counter_repeat;
  int event( int id );
  void action( int id );
};
