#pragma once

#include <Automaton.h>

class Atm_button : public Machine {
 public:
  enum { IDLE, WAIT, PRESSED, REPEAT, RELEASE, LIDLE, LWAIT, LPRESSED, LRELEASE, WRELEASE, AUTO_ST };
  enum { EVT_LMODE, EVT_TIMER, EVT_DELAY, EVT_REPEAT, EVT_PRESS, EVT_RELEASE, EVT_COUNTER, EVT_AUTO, ELSE };
  enum { BTN_PASS4 = -2, BTN_PASS3 = -3, BTN_PASS2 = -2, BTN_PASS1 = -1, BTN_RELEASE = 0, BTN_PRESS1 = 1, BTN_PRESS2 = 2, BTN_PRESS3 = 3, BTN_PRESS4 = 4 };

  Atm_button( void ) : Machine(){};
  Atm_button& begin( int attached_pin );
  Atm_button& trace( Stream& stream );
  Atm_button& onPress( atm_cb_push_t callback, int idx = 0 );
  Atm_button& onPress( Machine& machine, int event = 0 );
  Atm_button& onPress( int id, atm_cb_push_t callback, int idx = 0 );
  Atm_button& onPress( int id, Machine& machine, int event = 0 );
  Atm_button& onRelease( atm_cb_push_t callback, int idx = 0 );
  Atm_button& onRelease( Machine& machine, int event = 0 );
  Atm_button& debounce( int delay );
  Atm_button& longPress( int max, int delay );
  Atm_button& repeat( int delay = 500, int speed = 50 );
  Atm_button& autoPress( int delay, int press = 1 );

 protected:
  enum { ENT_PRESS, ENT_RELEASE, ENT_LSTART, ENT_LCOUNT, ENT_LRELEASE, EXT_WRELEASE, ENT_AUTO };
  static const int DEBOUNCE = 5;
  atm_connector onpress, onrelease;
  atm_connector longpress[2];
  short pin;
  atm_timer_millis timer_debounce, timer_delay, timer_repeat, timer_auto;
  atm_counter counter_longpress;
  int longpress_max;
  int auto_press = 1;

  int event( int id );
  void action( int id );
};
