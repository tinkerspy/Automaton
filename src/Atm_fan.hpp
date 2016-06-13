#pragma once

#include <Automaton.h>

class Atm_fan : public Machine {
 public:
  enum { IDLE, SEND };       // STATES
  enum { EVT_INPUT, ELSE };  // EVENTS
  Atm_fan( void ) : Machine(){};
  Atm_fan& begin( void );
  Atm_fan& trace( Stream& stream );
  Atm_fan& trigger( int event );
  int state( void );
  Atm_fan& onInput( Machine& machine, int event = 0 );
  Atm_fan& onInput( atm_cb_push_t callback, int idx = 0 );

 private:
  enum { ENT_SEND };                // ACTIONS
  enum { ON_INPUT, CONN_MAX = 4 };  // CONNECTORS
  atm_connector connectors[CONN_MAX];
  int event( int id );
  void action( int id );
};

/*
Automaton::ATML::begin - Automaton Markup Language

<?xml version="1.0" encoding="UTF-8"?>
<machines>
  <machine name="Atm_fan">
    <states>
      <IDLE index="0" sleep="1">
        <EVT_INPUT>SEND</EVT_INPUT>
      </IDLE>
      <SEND index="1" on_enter="ENT_SEND">
        <ELSE>IDLE</ELSE>
      </SEND>
    </states>
    <events>
      <EVT_INPUT index="0"/>
    </events>
    <connectors>
      <INPUT autostore="1" broadcast="1" dir="PUSH" slots="4"/>
    </connectors>
    <methods>
    </methods>
  </machine>
</machines>

Automaton::ATML::end
*/
