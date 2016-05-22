#pragma once

#include <Automaton.h>

class Atm_button : public Machine {
 public:
  enum { IDLE, WAIT, PRESSED, REPEAT, RELEASE, LIDLE, LWAIT, LPRESSED, LRELEASE, WRELEASE, AUTO };
  enum { EVT_LMODE, EVT_TIMER, EVT_DELAY, EVT_REPEAT, EVT_PRESS, EVT_RELEASE, EVT_COUNTER, EVT_AUTO, ELSE };
  enum { BTN_PASS4 = -2, BTN_PASS3 = -3, BTN_PASS2 = -2, BTN_PASS1 = -1, BTN_RELEASE = 0, BTN_PRESS1 = 1, BTN_PRESS2 = 2, BTN_PRESS3 = 3, BTN_PRESS4 = 4 };

  Atm_button( void ) : Machine(){};
  Atm_button& begin( int attached_pin );
  Atm_button& trace( Stream& stream );
  Atm_button& onPress( atm_cb_push_t callback, int idx = 0 );
  Atm_button& onPress( Machine& machine, int event = 0 );
  Atm_button& onRelease( atm_cb_push_t callback, int idx = 0 );
  Atm_button& onRelease( Machine& machine, int event = 0 );
  Atm_button& debounce( int delay );
  Atm_button& longPress( int max, int delay );
  Atm_button& repeat( int delay = 500, int speed = 50 );
  Atm_button& autoPress( int delay, int press = 1 );

 protected:
  enum { ACT_PRESS, ACT_RELEASE, ACT_LSTART, ACT_LCOUNT, ACT_LRELEASE, ACT_WRELEASE, ACT_AUTO };
  static const int _DEBOUNCE = 5;
  atm_connector _onpress, _onrelease;
  short _pin;
  atm_timer_millis _timer_debounce, _timer_delay, _timer_repeat, _timer_auto;
  atm_counter _counter_longpress;
  int _longpress_max;
  int _auto_press = 1;

  int event( int id );
  void action( int id );
};
