#pragma once

#include <Automaton.h>

class Atm_led : public Machine {
 public:
  enum { IDLE, ON, START, BLINK_OFF, LOOP, DONE, OFF, WT_ON, WT_START };
  enum { EVT_ON_TIMER, EVT_OFF_TIMER, EVT_WT_TIMER, EVT_COUNTER, EVT_ON, EVT_OFF, EVT_BLINK, EVT_TOGGLE, EVT_TOGGLE_BLINK, ELSE, EVT_BRUP, EVT_BRDN }; // BRUP/BRDN pseudo
  enum { EVT_START = EVT_BLINK };

  Atm_led( void ) : Machine(){};
  Atm_led& begin( int attached_pin, bool activeLow = false );
  Atm_led& blink( void );
  Atm_led& blink( uint32_t duration );
  Atm_led& blink( uint32_t duration, uint32_t pause_duration, uint16_t repeat_count = ATM_COUNTER_OFF );
  Atm_led& pause( uint32_t duration );
  Atm_led& fade( int fade );
  Atm_led& lead( uint32_t ms );
  Atm_led& repeat( uint16_t repeat );
  int brightness( int level = -1 );
  Atm_led& on( void );
  Atm_led& off( void );
  Atm_led& toggle( void );
  Atm_led& toggleBlink( void );
  Atm_led& start( void );
  Atm_led& trace( Stream& stream );
  Atm_led& onFinish( Machine& machine, int event = 0 );
  Atm_led& onFinish( atm_cb_push_t callback, int idx = 0 );
  Atm_led& range( int toLow, int toHigh, bool wrap = false );
  Atm_led& levels( unsigned char* map, int mapsize, bool wrap = false );
  int brighten( int v = 1 );
  Atm_led& trigger( int event );

 private:
  enum { ENT_INIT, ENT_ON, ENT_OFF, EXT_CHAIN };
  uint8_t level;
  short pin;
  bool activeLow;
  uint8_t toHigh, toLow;
  bool wrap;
  uint16_t repeat_count;
  atm_timer_millis on_timer, off_timer, lead_timer;
  atm_counter counter;
  atm_connector onfinish;
  unsigned char* levelMap;
  int levelMapSize;  
  int mapLevel( int level );


  int event( int id );
  void action( int id );
};
