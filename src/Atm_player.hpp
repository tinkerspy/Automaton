#pragma once

#include <Automaton.h>

class Atm_player : public Machine {
 public:
  enum { IDLE, START, SOUND, QUIET, NEXT, REPEAT, FINISH };                          // STATES
  enum { EVT_START, EVT_STOP, EVT_TOGGLE, EVT_TIMER, EVT_EOPAT, EVT_REPEAT, ELSE };  // EVENTS
  Atm_player( void ) : Machine(){};
  Atm_player& begin( int pin = -1 );
  Atm_player& trace( Stream& stream );
  Atm_player& trigger( int event );
  int state( void );
  Atm_player& play( int* pat, int size );
  Atm_player& play( uint32_t* pat, int size );
  Atm_player& play( int freq, int period, int pause = 0 );
  Atm_player& repeat( uint16_t v = -1 );
  Atm_player& speed( float v );
  Atm_player& pitch( float v );
  Atm_player& start( void );
  Atm_player& stop( void );
  Atm_player& toggle( void );
  Atm_player& onFinish( Machine& machine, int event = 0 );
  Atm_player& onFinish( atm_cb_push_t callback, int idx = 0 );
  Atm_player& onNote( Machine& machine, int event = 0 );
  Atm_player& onNote( atm_cb_push_t callback, int idx = 0 );
  Atm_player& onNote( int sub, Machine& machine, int event = 0 );
  Atm_player& onNote( int sub, atm_cb_push_t callback, int idx = 0 );

 private:
  int pin;
  int* pattern16; // Can also be 32 bit on some hardware (teensy 3.x)
  uint32_t* pattern32;
  uint16_t patternsize;
  uint8_t patternwidth;
  int step;
  uint16_t repeatCount;
  float speedFactor, pitchFactor;
  int stub[3];
  atm_timer_millis timer;
  atm_counter counter_repeat;
  enum { ENT_IDLE, ENT_START, ENT_SOUND, ENT_QUIET, ENT_NEXT, ENT_REPEAT, ENT_FINISH };  // ACTIONS
  enum { ON_FINISH, ON_NOTE, CONN_MAX = 3 };                                             // CONNECTORS
  atm_connector connectors[CONN_MAX];
  int event( int id );
  void action( int id );
};

/*
Automaton::ATML::begin - Automaton Markup Language

<?xml version="1.0" encoding="UTF-8"?>
<machines>
  <machine name="Atm_player">
    <states>
      <IDLE index="0" sleep="1" on_enter="ENT_IDLE">
        <EVT_START>START</EVT_START>
        <EVT_TOGGLE>START</EVT_TOGGLE>
      </IDLE>
      <START index="1" on_enter="ENT_START">
        <ELSE>SOUND</ELSE>
      </START>
      <SOUND index="2" on_enter="ENT_SOUND">
        <EVT_STOP>IDLE</EVT_STOP>
        <EVT_TOGGLE>IDLE</EVT_TOGGLE>
        <EVT_TIMER>QUIET</EVT_TIMER>
      </SOUND>
      <QUIET index="3" on_enter="ENT_QUIET">
        <EVT_STOP>IDLE</EVT_STOP>
        <EVT_TOGGLE>IDLE</EVT_TOGGLE>
        <EVT_TIMER>NEXT</EVT_TIMER>
      </QUIET>
      <NEXT index="4" on_enter="ENT_NEXT">
        <EVT_STOP>IDLE</EVT_STOP>
        <EVT_TOGGLE>IDLE</EVT_TOGGLE>
        <EVT_EOPAT>REPEAT</EVT_EOPAT>
        <ELSE>SOUND</ELSE>
      </NEXT>
      <REPEAT index="5" on_enter="ENT_REPEAT">
        <EVT_STOP>IDLE</EVT_STOP>
        <EVT_TOGGLE>IDLE</EVT_TOGGLE>
        <EVT_REPEAT>FINISH</EVT_REPEAT>
        <ELSE>START</ELSE>
      </REPEAT>
      <FINISH index="6" on_enter="ENT_FINISH">
        <EVT_REPEAT>IDLE</EVT_REPEAT>
        <ELSE>START</ELSE>
      </FINISH>
    </states>
    <events>
      <EVT_START index="0"/>
      <EVT_STOP index="1"/>
      <EVT_TOGGLE index="2"/>
      <EVT_TIMER index="3"/>
      <EVT_EOPAT index="4"/>
      <EVT_REPEAT index="5"/>
    </events>
    <connectors>
      <FINISH autostore="0" broadcast="0" dir="PUSH" slots="1"/>
      <NOTE autostore="0" broadcast="0" dir="PUSH" slots="2"/>
    </connectors>
    <methods>
    </methods>
  </machine>
</machines>

Automaton::ATML::end
*/
