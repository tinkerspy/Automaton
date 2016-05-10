#ifndef Atm_comparator_h
#define Atm_comparator_h

#include <Automaton.h>

typedef void ( *triggercb_t )( int v, int up, int idx_threshold, int v_threshold );

class Atm_comparator : public Machine {
 public:
  Atm_comparator( void ) : Machine() { class_label = "CMP"; };

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
  uint8_t _upmode, _downmode;
  union {
    struct {
      Machine* _up_machine;
      state_t _up_machine_event;
    };
    struct {
      TinyMachine* _up_tmachine;
      state_t _up_tmachine_event;
    };
    struct {
      const char* _up_label;
      state_t _up_label_event;
    };
  };
  union {
    struct {
      Machine* _down_machine;
      state_t _down_machine_event;
    };
    struct {
      TinyMachine* _down_tmachine;
      state_t _down_tmachine_event;
    };
    struct {
      const char* _down_label;
      state_t _down_label_event;
    };
  };

  void ( *callback )( int v, int up, int idx_threshold, int v_threshold ) = 0;

  enum { IDLE, SAMPLE, SEND } STATES;
  enum { EVT_TRIGGER, EVT_TIMER, ELSE } EVENTS;
  enum { ACT_SAMPLE, ACT_SEND } ACTIONS;
  enum { MODE_MACHINE, MODE_TMACHINE, MODE_FACTORY };

  Atm_comparator& begin( int attached_pin, int blinkrate, triggercb_t callback = 0 );
  Atm_comparator& threshold( uint16_t* v, uint16_t size, bool catchUp = false );
  Atm_comparator& average( uint16_t* v, uint16_t size );
  Atm_comparator& onUp( Machine& machine, int event = 0 );
  Atm_comparator& onUp( TinyMachine& machine, int event = 0 );
  Atm_comparator& onUp( const char* label, int event = 0 );
  Atm_comparator& onDown( Machine& machine, int event = 0 );
  Atm_comparator& onDown( TinyMachine& machine, int event = 0 );
  Atm_comparator& onDown( const char* label, int event = 0 );
  int _avg();
  Atm_comparator& bitmap( uint16_t v );
  int sample();
  virtual int read_sample();
  int event( int id );
  void action( int id );
  Atm_comparator& trace( Stream& stream );
};

class Att_comparator : public TinyMachine {
 public:
  Att_comparator( void ) : TinyMachine(){};

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
  uint8_t _upmode, _downmode;
  union {
    struct {
      Machine* _up_machine;
      state_t _up_machine_event;
    };
    struct {
      TinyMachine* _up_tmachine;
      state_t _up_tmachine_event;
    };
    struct {
      const char* _up_label;
      state_t _up_label_event;
    };
  };
  union {
    struct {
      Machine* _down_machine;
      state_t _down_machine_event;
    };
    struct {
      TinyMachine* _down_tmachine;
      state_t _down_tmachine_event;
    };
    struct {
      const char* _down_label;
      state_t _down_label_event;
    };
  };

  void ( *callback )( int v, int up, int idx_threshold, int v_threshold ) = 0;

  enum { IDLE, SAMPLE, SEND } STATES;
  enum { EVT_TRIGGER, EVT_TIMER, ELSE } EVENTS;
  enum { ACT_SAMPLE, ACT_SEND } ACTIONS;
  enum { MODE_MACHINE, MODE_TMACHINE, MODE_FACTORY };

  Att_comparator& begin( int attached_pin, int blinkrate, triggercb_t callback = 0 );
  Att_comparator& threshold( uint16_t* v, uint16_t size, bool catchUp = false );
  Att_comparator& average( uint16_t* v, uint16_t size );
  Att_comparator& onUp( Machine& machine, int event = 0 );
  Att_comparator& onDown( Machine& machine, int event = 0 );
  Att_comparator& onUp( TinyMachine& machine, int event = 0 );
  Att_comparator& onDown( TinyMachine& machine, int event = 0 );
  int _avg();
  Att_comparator& bitmap( uint16_t v );
  int sample();
  virtual int read_sample();
  int event( int id );
  void action( int id );
};

#endif