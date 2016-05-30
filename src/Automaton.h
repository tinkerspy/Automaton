/*
  Automaton.h - Reactive State Machine Framework for Arduino.
  Published under the MIT License (MIT), Copyright (c) 2015-2016, J.P. van der Landen
*/

#pragma once

#include "Arduino.h"

typedef int8_t state_t;

const uint8_t ATM_SLEEP_FLAG = B00000001;
const uint8_t ATM_CYCLE_FLAG = B00000010;
const uint8_t ATM_USR1_FLAG = B00010000;
const uint8_t ATM_USR2_FLAG = B00100000;
const uint8_t ATM_USR3_FLAG = B01000000;
const uint8_t ATM_USR4_FLAG = B10000000;
const uint8_t ATM_USR_FLAGS = B11110000;

#define read_state( addr ) ( state_t ) pgm_read_byte_near( addr )

class Appliance;
class Machine;

typedef void ( *swcb_sym_t )( Stream* stream, Machine& machine, const char label[], const char current[], const char next[], const char trigger[],
                              uint32_t runtime, uint32_t cycles );

const uint8_t ATM_UP = 1;
const uint8_t ATM_DOWN = 0;

const state_t ATM_NOP = -1;
const state_t ATM_SLEEP = -2;
const state_t ATM_ON_SWITCH = -3;
const state_t ATM_ON_ENTER = 0;
const state_t ATM_ON_LOOP = 1;
const state_t ATM_ON_EXIT = 2;

const uint32_t ATM_TIMER_OFF = 0xffffffff;  // This timer value never expires
const uint16_t ATM_COUNTER_OFF = 0xffff;    // This counter value never expires

class atm_serial_debug {  // It seems necessary to put this code in .h to keep it from being compiled in unnecessarily
 public:
  static void trace( Stream* stream, Machine& machine, const char label[], const char current[], const char next[], const char trigger[], uint32_t runtime,
                     uint32_t cycles ) {
    stream->print( millis() );
    stream->print( " Switch " );
    stream->print( label );
    stream->print( "@" );
    stream->print( (long)&machine, HEX );
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
  int expired( Machine* machine );
};

class atm_counter {
 public:
  uint16_t value;
  void set( uint16_t v );
  uint8_t expired( void );
  uint16_t decrement( void );
};

typedef void ( *atm_cb_push_t )( int idx, int v, int up );
typedef bool ( *atm_cb_pull_t )( int idx );

class atm_connector {
 public:
  enum { MODE_NULL, MODE_PUSHCB, MODE_PULLCB, MODE_MACHINE };            // bits 0, 1, 2 - Mode
  enum { LOG_AND, LOG_OR, LOG_XOR };                                     // bits 3, 4    - Logical operator
  enum { REL_NULL, REL_EQ, REL_NEQ, REL_LT, REL_GT, REL_LTE, REL_GTE };  // bits 5, 6, 7 - Relational operator
  uint8_t mode_flags;
  union {
    struct {
      union {
        atm_cb_push_t push_callback;
        atm_cb_pull_t pull_callback;
      };
      int callback_idx;  // +2 = 5 bytes per connector/object
    };
    struct {
      union {
        Machine* machine;
      };
      int event;
    };
  };
  void set( Machine* m, int evt, int8_t logOp = 0, int8_t relOp = 0 );
  void set( atm_cb_push_t callback, int idx, int8_t logOp = 0, int8_t relOp = 0 );
  void set( atm_cb_pull_t callback, int idx, int8_t logOp = 0, int8_t relOp = 0 );
  bool push( int v = 0, int up = 0, bool noCallback = false );  // returns false (only) if callback is set!
  int pull( int v = 0, int up = 0, bool def_value = false );
  int8_t logOp( void );
  int8_t relOp( void );
  int8_t mode( void );
};

class Machine {
 public:
  virtual int state( void );
  virtual Machine& trigger( int evt = 0 );
  Machine& cycle( uint32_t time = 0 );
  uint32_t state_millis;
  uint8_t flags = ATM_SLEEP_FLAG;
  state_t next_trigger = -1;
  uint8_t sleep( int8_t v = -1 );
  virtual int event( int id ) = 0;  // Pure virtual methods -> make this an abstract class
  virtual void action( int id ) = 0;

  Machine* inventory_next;

 protected:
  Machine& state( int state );
  Machine& begin( const state_t tbl[], int width );
  const char* mapSymbol( int id, const char map[] );
  Machine& setTrace( Stream* stream, swcb_sym_t callback, const char symbols[] );

  const state_t* state_table;
  state_t next;
  state_t current = -1;
  state_t last_trigger = -1;
  const char* symbols;
  uint8_t state_width;
  swcb_sym_t callback_trace;
  Stream* stream_trace;
  uint32_t cycles;
};

class Appliance {
 public:
  Appliance& component( Machine& machine );
  Appliance& run( uint32_t time = 0 );

 private:
  Machine* inventory_root;
  void runTiny( void );
};

// Bundled state machines (components)

#include <Atm_analog.hpp>
#include <Atm_bit.hpp>
#include <Atm_button.hpp>
#include <Atm_command.hpp>
#include <Atm_comparator.hpp>
#include <Atm_controller.hpp>
#include <Atm_digital.hpp>
#include <Atm_encoder.hpp>
#include <Atm_fade.hpp>
#include <Atm_led.hpp>
#include <Atm_fan.hpp>
#include <Atm_step.hpp>
#include <Atm_player.hpp>
#include <Atm_timer.hpp>
