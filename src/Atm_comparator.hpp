#pragma once

#include <Automaton.h>

class Atm_comparator : public Machine {
 public:
  enum { IDLE, SAMPLE, SEND };            // STATES
  enum { EVT_TRIGGER, EVT_TIMER, ELSE };  // EVENTS

  Atm_comparator( void ) : Machine(){};
  Atm_comparator& begin( int attached_pin, int sampleRate = 50 );
  Atm_comparator& threshold( uint16_t* v, uint16_t size, bool catchUp = false );
  Atm_comparator& average( uint16_t* v, uint16_t size );
  Atm_comparator& skip();
  Atm_comparator& onChange( atm_cb_push_t callback, int idx = 0 );
  Atm_comparator& onChange( Machine& machine, int event = 0 );
  Atm_comparator& onChange( bool status, atm_cb_push_t callback, int idx = 0 );
  Atm_comparator& onChange( bool status, Machine& machine, int event = 0 );
  int state( void );
  virtual int read_sample();
  Atm_comparator& trace( Stream& stream );

 private:
  enum { ENT_SAMPLE, ENT_SEND };  // ACTIONS
  short pin;
  atm_timer_millis timer;
  int v_sample, v_threshold, v_previous;
  uint64_t bitmap_sample, bitmap_previous, bitmap_diff;
  uint16_t* p_threshold;  // Max 64 values
  uint16_t p_threshold_size;
  uint16_t* avg_buf;
  uint16_t avg_buf_size;
  uint16_t avg_buf_head;
  uint32_t avg_buf_total;
  atm_connector onup, ondown;
  int skip_mode;

  int avg();
  Atm_comparator& bitmap( uint16_t v );
  int sample();
  int event( int id );
  void action( int id );
};
