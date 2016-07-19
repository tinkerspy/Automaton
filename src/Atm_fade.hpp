#pragma once

#include <Automaton.h>

class Atm_fade : public Machine {
 public:
  enum { IDLE, ON, START, STARTU, UP, STARTD, DOWN, REPEAT };
  enum { EVT_CNT_FADE, EVT_TM_FADE, EVT_TM_ON, EVT_TM_OFF, EVT_CNT_RPT, EVT_ON, EVT_OFF, EVT_BLINK, EVT_TOGGLE, EVT_TOGGLE_BLINK, ELSE };

  Atm_fade( void ) : Machine(){};
  Atm_fade& begin( int attached_pin );
  Atm_fade& trace( Stream& stream );
  Atm_fade& blink( uint32_t duration, uint32_t pause_duration, uint16_t repeat_count = ATM_COUNTER_OFF );
  Atm_fade& blink( uint32_t duration );
  Atm_fade& pause( uint32_t duration );
  Atm_fade& fade( int fade );
  Atm_fade& fade( unsigned long time_ms, uint8_t pwm_first, uint8_t pwm_last); // fade an LED with a range of PWM value over a duration of milliseconds
  Atm_fade& repeat( uint16_t repeat );
  Atm_fade& on( void );
  Atm_fade& off( void );
  Atm_fade& toggle( void );
  Atm_fade& toggleBlink( void );
  Atm_fade& start( void );

 private:
  enum { ENT_REPEAT, ENT_OFF, ENT_ON, ENT_UP, ENT_DOWN, ENT_START };
  uint8_t fade_first;
  uint8_t fade_last;
  unsigned long fade_length; // the millisecond time from fade_first to fade_last, OR back from fade_last to fade_first
  unsigned long fade_accu; // the accumulate time from the start. <= fade_length
  unsigned long fade_prev_tm; // the last update time, for fade_accu
  short pin;
  uint16_t repeat_count;
  atm_timer_millis timer_fade, timer_on, timer_off;
  atm_counter counter_repeat;
  int event( int id );
  void action( int id );
};
