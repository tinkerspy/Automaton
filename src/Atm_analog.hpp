#pragma once

#include <Automaton.h>

class Atm_analog : public Machine {
 public:
  enum { IDLE, SAMPLE, SEND };            // STATES
  enum { EVT_TRIGGER, EVT_TIMER, ELSE };  // EVENTS

  Atm_analog( void ) : Machine(){};
  Atm_analog& begin( int attached_pin, int sampleRate = 50 );
  Atm_analog& average( uint16_t* v, uint16_t size );
  Atm_analog& trace( Stream& stream );
  int state( void );
  Atm_analog& range( int toLow, int toHigh );
  Atm_analog& onChange( Machine& machine, int event = 0 );
  Atm_analog& onChange( atm_cb_push_t callback, int idx = 0 );
  Atm_analog& set( int value );

 private:
  enum { ENT_SAMPLE, ENT_SEND };  // ACTIONS
  short pin;
  atm_timer_millis timer;
  int v_sample, v_threshold, v_previous;
  atm_connector onchange;
  uint16_t* avg_buf;
  uint16_t avg_buf_size;
  uint16_t avg_buf_head;
  uint32_t avg_buf_total;
  int toLow, toHigh;

  int avg();
  int sample();
  virtual int read_sample();
  int event( int id );
  void action( int id );
};
