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

class Atm_fade : public MACHINE {
 public:
  Atm_fade( void ) : MACHINE() {
#ifndef TINYMACHINE
    class_label = "FAD";
#endif
  };

  enum { IDLE, ON, START, STARTU, UP, STARTD, DOWN, REPEAT };
  enum { EVT_CNT_FADE, EVT_TM_FADE, EVT_TM_ON, EVT_TM_OFF, EVT_CNT_RPT, EVT_ON, EVT_OFF, EVT_BLINK, EVT_TOGGLE, EVT_TOGGLE_BLINK, ELSE };
  enum { ACT_REPEAT, ACT_OFF, ACT_ON, ACT_UP, ACT_DOWN, ACT_START };

  static const uint8_t SLOPE_SIZE = 32;
  uint8_t slope[SLOPE_SIZE] = {0, 0, 1, 1, 2, 2, 3, 4, 5, 6, 7, 8, 10, 12, 15, 18, 22, 26, 31, 37, 44, 54, 63, 76, 90, 108, 127, 153, 180, 217, 230, 255};
  short pin;
  int repeat_count;
  atm_timer_millis timer_fade, timer_on, timer_off;
  atm_counter counter_fade, counter_repeat;

  Atm_fade& begin( int attached_pin );
  Atm_fade& trace( Stream& stream );
  Atm_fade& blink( uint32_t duration, uint32_t pause_duration, uint16_t repeat_count = ATM_COUNTER_OFF );
  Atm_fade& blink( uint32_t duration );
  Atm_fade& pause( uint32_t duration );
  Atm_fade& fade( int fade );
  Atm_fade& repeat( int repeat );
  int event( int id );
  void action( int id );
};
