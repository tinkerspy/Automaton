#pragma once

#include <Automaton.h>
#include <Servo.h>

class Atm_servo: public Machine {

 public:
  enum { IDLE, UP, UP_NEXT, DOWN, DOWN_NEXT, FINISHED }; // STATES
  enum { EVT_UP, EVT_DOWN, EVT_TIMER, ELSE }; // EVENTS
  Atm_servo( void ) : Machine() {};
  Atm_servo& begin( int pin, int pos = 0 );
  Atm_servo& trace( Stream & stream );
  Atm_servo& trigger( int event );
  int state( void );
  Atm_servo& onChange( Machine& machine, int event = 0 );
  Atm_servo& onChange( atm_cb_push_t callback, int idx = 0 );
  Atm_servo& onChange( int sub, Machine& machine, int event = 0 );
  Atm_servo& onChange( int sub, atm_cb_push_t callback, int idx = 0 );
  Atm_servo& onFinish( Machine& machine, int event = 0 );
  Atm_servo& onFinish( atm_cb_push_t callback, int idx = 0 );
  Atm_servo& step( int degrees, int time = 0 );
  Atm_servo& position( int pos );
  int position( void );

 private:
  enum { ENT_IDLE, ENT_UP, ENT_DOWN, ENT_FINISHED }; // ACTIONS
  enum { ON_CHANGE, ON_FINISH = 2, CONN_MAX }; // CONNECTORS
  atm_connector connectors[CONN_MAX];
  int pin;
  Servo servo;
  atm_timer_millis timer;
  int servo_pos, servo_dest;
  int step_size, step_time;
  int event( int id ); 
  void action( int id ); 

};

/*
Automaton::ATML::begin - Automaton Markup Language

<?xml version="1.0" encoding="UTF-8"?>
<machines>
  <machine name="Atm_servo">
    <states>
      <IDLE index="0" sleep="1" on_enter="ENT_IDLE">
        <EVT_UP>UP</EVT_UP>
        <EVT_DOWN>DOWN</EVT_DOWN>
      </IDLE>
      <UP index="1" on_enter="ENT_UP">
        <EVT_TIMER>UP_NEXT</EVT_TIMER>
      </UP>
      <UP_NEXT index="2">
        <EVT_UP>UP</EVT_UP>
        <EVT_DOWN>DOWN</EVT_DOWN>
        <ELSE>FINISHED</ELSE>
      </UP_NEXT>
      <DOWN index="3" on_enter="ENT_DOWN">
        <EVT_TIMER>DOWN_NEXT</EVT_TIMER>
      </DOWN>
      <DOWN_NEXT index="4">
        <EVT_UP>UP</EVT_UP>
        <EVT_DOWN>DOWN</EVT_DOWN>
        <ELSE>FINISHED</ELSE>
      </DOWN_NEXT>
      <FINISHED index="5" on_enter="ENT_FINISHED">
        <ELSE>IDLE</ELSE>
      </FINISHED>
    </states>
    <events>
      <EVT_UP index="0"/>
      <EVT_DOWN index="1"/>
      <EVT_TIMER index="2"/>
    </events>
    <connectors>
      <CHANGE autostore="0" broadcast="0" dir="PUSH" slots="2"/>
      <FINISH autostore="0" broadcast="0" dir="PUSH" slots="1"/>
    </connectors>
    <methods>
    </methods>
  </machine>
</machines>

Automaton::ATML::end
*/

