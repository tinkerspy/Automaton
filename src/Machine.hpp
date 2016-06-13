/*
  Automaton.h - Reactive State Machine Framework for Arduino.
  Published under the MIT License (MIT), Copyright (c) 2015-2016, J.P. van der Landen
*/

#pragma once

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
  void onPush( atm_connector connectors[], int id, int sub, int slots, int fill, Machine& machine, int event );
  void onPush( atm_connector connectors[], int id, int sub, int slots, int fill, atm_cb_push_t callback, int idx );
  void push( atm_connector connectors[], int id, int sub, int v, int up );

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
