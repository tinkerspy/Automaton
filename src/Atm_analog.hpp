#pragma once

#include <Automaton.h>

#undef TINYMACHINE
#undef FACTORY
#undef STATE_TYPE
#undef MACHINE

#ifdef TINYMACHINE
#define MACHINE TinyMachine
#define FACTORY 0
#define STATE_TYPE tiny_state_t
#else
#define MACHINE Machine
#define FACTORY factory
#define STATE_TYPE state_t
#endif

typedef bool ( *atm_analog_cb_t )( int idx, int v, int up );

class Atm_analog : public MACHINE {
 public:
  Atm_analog( void ) : MACHINE() {
#ifndef TINYMACHINE
    class_label = "ANI";
#endif
  };

  short pin;
  atm_timer_millis timer;
  int v_sample, v_threshold, v_previous;
  atm_connector _onchange;

  uint16_t* avg_buf;
  uint16_t avg_buf_size;
  uint16_t avg_buf_head;
  uint32_t avg_buf_total;
  uint16_t _toLow, _toHigh;

  enum { IDLE, SAMPLE, SEND } STATES;
  enum { EVT_TRIGGER, EVT_TIMER, ELSE } EVENTS;
  enum { ACT_SAMPLE, ACT_SEND } ACTIONS;
  enum { MODE_MACHINE, MODE_TMACHINE, MODE_FACTORY };

  Atm_analog& begin( int attached_pin, int sampleRate = 50 );
  Atm_analog& average( uint16_t* v, uint16_t size );
  Atm_analog& trace( Stream& stream );
  int state( void );
  Atm_analog& range( int toLow, int toHigh );

  Atm_analog& onChange( Machine& machine, int event = 0 );
  Atm_analog& onChange( TinyMachine& machine, int event = 0 );
  Atm_analog& onChange( atm_analog_cb_t callback, int idx = 0 );
  Atm_analog& onChange( const char* label, int event = 0 );

 private:
  int _avg();
  int sample();
  virtual int read_sample();
  int event( int id );
  void action( int id );
};
