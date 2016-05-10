#pragma once

#include <Automaton.h>

#define TINYMACHINE

#ifdef TINYMACHINE
#define MACHINE TinyMachine
#define FACTORY 0
#define STATE_TYPE tiny_state_t
#else
#define MACHINE Machine
#define FACTORY factory
#define STATE_TYPE state_t
#endif

class Att_led : public MACHINE {
 public:
  Att_led( void ) : MACHINE() {
#ifndef TINYMACHINE      
    class_label = "LED";
#endif    
  };

  enum { IDLE, ON, START, BLINK_OFF, DONE, OFF };
  enum { EVT_ON_TIMER, EVT_OFF_TIMER, EVT_COUNTER, EVT_ON, EVT_OFF, EVT_BLINK, EVT_TOGGLE, EVT_TOGGLE_BLINK, ELSE };
  enum { ACT_INIT, ACT_ON, ACT_OFF, ACT_CHAIN };

  Att_led& begin( int attached_pin, bool activeLow = false );
  Att_led& blink( uint32_t duration );
  Att_led& blink( uint32_t duration, uint32_t pause_duration, uint16_t repeat_count = ATM_COUNTER_OFF );
  Att_led& pause( uint32_t duration );
  Att_led& fade( int fade );
  Att_led& repeat( int repeat );
  Att_led& trace( Stream& stream );
  Att_led& onFinish( Machine& machine, int16_t event = 0 );
  Att_led& onFinish( TinyMachine& machine, int16_t event = 0 );
  Att_led& onFinish( atm_cb_t callback, int16_t idx = 0 );
  Att_led& onFinish( const char* label, int16_t event = 0 );

  
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

