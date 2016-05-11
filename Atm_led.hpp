#pragma once

#include <Automaton.h>

#undef TINYMACHINE

#ifdef TINYMACHINE
#define MACHINE TinyMachine
#define FACTORY 0
#define STATE_TYPE tiny_state_t
#else
#define MACHINE Machine
#define FACTORY factory
#define STATE_TYPE state_t
#endif

class Atm_led : public MACHINE {
 public:
  Atm_led( void ) : MACHINE() {
#ifndef TINYMACHINE      
    class_label = "LED";
#endif    
  };

  enum { IDLE, ON, START, BLINK_OFF, DONE, OFF };
  enum { EVT_ON_TIMER, EVT_OFF_TIMER, EVT_COUNTER, EVT_ON, EVT_OFF, EVT_BLINK, EVT_TOGGLE, EVT_TOGGLE_BLINK, ELSE };
  enum { ACT_INIT, ACT_ON, ACT_OFF, ACT_CHAIN };

  Atm_led& begin( int attached_pin, bool activeLow = false );
  Atm_led& blink( uint32_t duration );
  Atm_led& blink( uint32_t duration, uint32_t pause_duration, uint16_t repeat_count = ATM_COUNTER_OFF );
  Atm_led& pause( uint32_t duration );
  Atm_led& fade( int fade );
  Atm_led& repeat( int repeat );
  Atm_led& trace( Stream& stream );
  Atm_led& onFinish( Machine& machine, int event = 0 );
  Atm_led& onFinish( TinyMachine& machine, int event = 0 );
  Atm_led& onFinish( atm_cb_t callback, int idx = 0 );
  Atm_led& onFinish( const char* label, int event = 0 );

  
 private:
  short pin;
  bool _activeLow;
  int repeat_count;
  atm_timer_millis on_timer, off_timer;
  atm_counter counter;
  atm_connector _onfinish;

  int event( int id );
  void action( int id );
};

