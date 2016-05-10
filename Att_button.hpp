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


typedef void ( *atm_button_onpress_cb_t )( int press, int idx );

class Att_button : public MACHINE {
 public:
  Att_button( void ) : MACHINE() {
#ifndef TINYMACHINE      
    class_label = "BTN";
#endif    
  };
  
  enum { IDLE, WAIT, PRESSED, REPEAT, RELEASE, LIDLE, LWAIT, LPRESSED, LRELEASE, WRELEASE, AUTO };
  enum { EVT_LMODE, EVT_TIMER, EVT_DELAY, EVT_REPEAT, EVT_PRESS, EVT_RELEASE, EVT_COUNTER, EVT_AUTO, ELSE };
  enum { ACT_PRESS, ACT_RELEASE, ACT_LSTART, ACT_LCOUNT, ACT_LRELEASE, ACT_WRELEASE, ACT_AUTO };
  enum { BTN_PASS4 = -2, BTN_PASS3 = -3, BTN_PASS2 = -2, BTN_PASS1 = -1, BTN_RELEASE = 0, BTN_PRESS1 = 1, BTN_PRESS2 = 2, BTN_PRESS3 = 3, BTN_PRESS4 = 4 };

  Att_button& begin( int attached_pin );
  Att_button& trace( Stream& stream );
  Att_button& onPress( atm_button_onpress_cb_t callback, int idx = 0 );
  Att_button& onPress( Machine& machine, int event = 0 );
  Att_button& onPress( TinyMachine& machine, int event = 0 );
  Att_button& onPress( const char* label, int event = 0 );
  Att_button& debounce( int delay );
  Att_button& longPress( int max, int delay );
  Att_button& repeat( int delay = 500, int speed = 50 );
  Att_button& autoPress( int delay, int press = 1 );

 protected:
  int event( int id );
  void action( int id );
  void cb( int press, int id );
  static const int _DEBOUNCE = 5;
  atm_connector _onpress;
  short _pin;
  atm_timer_millis _timer_debounce, _timer_delay, _timer_repeat, _timer_auto;
  atm_counter _counter_longpress;
  int16_t _longpress_max;
  int16_t _auto_press = 1;
};


