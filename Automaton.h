/*
  Automaton.h - Library for creating and running Finite State Machines.
  Published under the MIT License (MIT), Copyright (c) 2015, J.P. van der Landen
*/

#pragma once

#include "Arduino.h"

typedef int16_t state_t;
typedef int8_t tiny_state_t;

const uint8_t ATM_SLEEP_FLAG = B00000001;
const uint8_t ATM_CYCLE_FLAG = B00000010;
const uint8_t ATM_USR1_FLAG = B00010000;
const uint8_t ATM_USR2_FLAG = B00100000;
const uint8_t ATM_USR3_FLAG = B01000000;
const uint8_t ATM_USR4_FLAG = B10000000;
const uint8_t ATM_USR_FLAGS = B11110000;

#define tiny_read_state( addr ) ( tiny_state_t ) pgm_read_byte_near( addr )
#define read_state( addr ) ( state_t ) pgm_read_word_near( addr )

typedef void ( *swcb_sym_t )( Stream* stream, const char label[], const char current[], const char next[], const char trigger[], uint32_t runtime,
                              uint32_t cycles );

const int8_t ATM_NO_OF_QUEUES = 5;  // queues 0, 1, 2, 3, 4
const int8_t ATM_DEFAULT_PRIO = 1;

const state_t ATM_NOP = -1;
const state_t ATM_SLEEP = -2;
const state_t ATM_ON_SWITCH = -3;
const state_t ATM_ON_ENTER = 0;
const state_t ATM_ON_LOOP = 1;
const state_t ATM_ON_EXIT = 2;

const uint32_t ATM_TIMER_OFF = 0xffffffff;  // This timer value never expires
const uint16_t ATM_COUNTER_OFF = 0xffff;    // This counter value never expires

class Factory;
class Machine;
class TinyMachine;
class BaseMachine;

class atm_serial_debug {
 public:
  static void trace( Stream* stream, const char label[], const char current[], const char next[], const char trigger[], uint32_t runtime, uint32_t cycles ) {
    stream->print( millis() );
    stream->print( " Switch " );
    stream->print( label );
    stream->print( " from " );
    stream->print( current );
    stream->print( " to " );
    stream->print( next );
    stream->print( " on " );
    stream->print( trigger );
    stream->print( " (" );
    stream->print( cycles );
    stream->print( " cycles in " );
    stream->print( runtime );
    stream->println( " ms)" );
  }
};

class atm_timer_millis {
 public:
  uint32_t value;
  void set( uint32_t v );
  int expired( BaseMachine* machine );
};

class atm_timer_micros {
 public:
  uint32_t value;
  void set( uint32_t v );
  int expired( Machine* machine );
};

class atm_counter {
 public:
  uint16_t value;
  void set( uint16_t v );
  uint8_t expired( void );
  uint16_t decrement( void );
};

class atm_pin {  // TODO untested!
  uint32_t pinstate;

 public:
  uint8_t change( uint8_t pin );
};

typedef bool ( *atm_cb_t )( int16_t idx );

class atm_connector {
 public:
  enum { MODE_NULL, MODE_CALLBACK, MODE_MACHINE, MODE_TMACHINE, MODE_FACTORY };  // bits 0, 1, 2
  enum { LOG_AND, LOG_OR, LOG_XOR };                                             // bits 3, 4 MOVE to condition
  enum { REL_NULL, REL_EQ, REL_NEQ, REL_LT, REL_GT, REL_LTE, REL_GTE };          // bits 5, 6, 7 Move condition
  uint8_t mode_flags;
  union {
    struct {
      atm_cb_t callback;
      int16_t callback_idx;  // +2 = 5 bytes per object
    };
    struct {
      union {
        Machine* machine;
        TinyMachine* tmachine;
        const char* label;
      };
      uint16_t event;
    };
  };
  void set( Machine* m, int16_t evt, int8_t logOp = 0, int8_t relOp = 0 );
  void set( TinyMachine* tm, int16_t evt, int8_t logOp = 0, int8_t relOp = 0 );
  void set( const char* l, int16_t evt, int8_t logOp = 0, int8_t relOp = 0 );
  void set( atm_cb_t cb, int16_t idx, int8_t logOp = 0, int8_t relOp = 0 );
  bool push( Factory* f = 0, bool noCallback = false );  // returns false (only) if callback is set!
  int16_t pull( Factory* f = 0, bool def_value = false );
  int8_t logOp( void );
  int8_t relOp( void );
  int8_t mode( void );
};

class BaseMachine {
 public:
  uint32_t state_millis;
  uint8_t flags = ATM_SLEEP_FLAG;
  state_t next_trigger = -1;

  uint8_t sleep( int8_t v = -1 );
  virtual int event( int id ) = 0;  // Pure virtual methods -> make this an abstract class
  virtual void action( int id ) = 0;
};

class Machine : public BaseMachine {
 public:
  state_t state( void );
  Machine& trigger( int evt = 0 );
  Machine& priority( int8_t priority );
  int8_t priority( void );
  Machine& cycle( uint32_t time = 0 );
  Machine& label( const char label[] );

  int8_t prio;
  uint32_t state_micros;
  const char* inst_label;
  const char* class_label;
  Machine* inventory_next;
  Machine* priority_next;
  Factory* factory;

 protected:
  Machine& state( state_t state );
  Machine& begin( const state_t tbl[], int width );
  const char* mapSymbol( int id, const char map[] );
  Machine& setTrace( Stream* stream, swcb_sym_t callback, const char symbols[] );

  const state_t* state_table;
  state_t next;
  state_t current = -1;
  state_t last_trigger = -1;
  const char* _symbols;
  uint8_t state_width;
  swcb_sym_t callback_trace;
  Stream* stream_trace;
  uint32_t cycles;
};

class TinyMachine : public BaseMachine {
 public:
  tiny_state_t state( void );
  TinyMachine& cycle( uint32_t time = 0 );
  TinyMachine& trigger( int evt = 0 );
  TinyMachine* inventory_next;

 protected:
  TinyMachine& state( tiny_state_t state );
  TinyMachine& begin( const tiny_state_t tbl[], int width );
  const tiny_state_t* state_table;
  tiny_state_t next = 0;
  tiny_state_t current = -1;
  uint8_t state_width;
};

class Factory {
 public:
  Factory& add( Machine& machine );
  Factory& add( TinyMachine& machine );
  Machine* find( const char label[] );
  Factory& trigger( const char label[], int event = 0 );
  int state( const char label[] );
  Factory& cycle( uint32_t time = 0 );

 private:
  int8_t recalibrate = 1;
  Machine* inventory_root;
  Machine* priority_root[ATM_NO_OF_QUEUES];
  TinyMachine* tiny_root;
  void calibrate( void );
  void run( int q );
  void runTiny( void );
};

#include <Atm_button.hpp>
#include <Att_button.hpp>
#include <Atm_command.hpp>
#include <Att_command.hpp>
#include <Atm_comparator.hpp>
#include <Atm_fade.hpp>
#include <Att_fade.hpp>
#include <Atm_gate.hpp>
#include <Atm_led.hpp>
#include <Att_led.hpp>
#include <Atm_multiplier.hpp>
#include <Att_multiplier.hpp>
#include <Atm_pulse.hpp>
#include <Att_pulse.hpp>
#include <Atm_step.hpp>
#include <Att_step.hpp>
#include <Atm_timer.hpp>
#include <Att_timer.hpp>
#include <Atm_bit.hpp>
#include <Att_bit.hpp>
#include <Atm_condition.hpp>
#include <Att_condition.hpp>
