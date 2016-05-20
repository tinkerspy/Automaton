/*
  Automaton.cpp - Library for creating and running Finite State Machines.
  Published under the MIT License (MIT), Copyright (c) 2015, J.P. van der Landen
*/

#include "Automaton.h"

bool atm_connector::push( bool noCallback /* = false */ ) {
  switch ( mode_flags & B00000111 ) {
    case MODE_CALLBACK:
      if ( noCallback ) {
        return false;
      } else {
        ( *callback )( callback_idx );
      }
      return true;
    case MODE_Machine:
      machine->trigger( event );
      return true;
  }
  return true;
}

int atm_connector::pull( bool def_value /* = false */ ) {
  switch ( mode_flags & B00000111 ) {
    case MODE_CALLBACK:
      return ( *callback )( callback_idx );
    case MODE_Machine:
      return machine->state();
  }
  return def_value;
}

int8_t atm_connector::logOp( void ) {
  return ( mode_flags & B00011000 ) >> 3;
}

int8_t atm_connector::relOp( void ) {
  return ( mode_flags & B11100000 ) >> 5;
}

void atm_connector::set( atm_cb_t cb, int idx, int8_t logOp /* = 0 */, int8_t relOp /* = 0 */ ) {
  mode_flags = MODE_CALLBACK | ( logOp << 3 ) | ( relOp << 5 );
  callback = cb;
  callback_idx = idx;
}

void atm_connector::set( Machine* m, int evt, int8_t logOp /* = 0 */, int8_t relOp /* = 0 */ ) {
  mode_flags = MODE_Machine | ( logOp << 3 ) | ( relOp << 5 );
  machine = m;
  event = evt;
}

int8_t atm_connector::mode( void ) {
  return mode_flags & B00000111;
}

void atm_timer_millis::set( uint32_t v ) {
  value = v;
}

int atm_timer_millis::expired( Machine* machine ) {
  return value == ATM_TIMER_OFF ? 0 : millis() - machine->state_millis >= value;
}

void atm_counter::set( uint16_t v ) {
  value = v;
}

uint16_t atm_counter::decrement( void ) {
  return value > 0 && value != ATM_COUNTER_OFF ? --value : 0;
}

uint8_t atm_counter::expired() {
  return value == ATM_COUNTER_OFF ? 0 : ( value > 0 ? 0 : 1 );
}

Machine& Machine::state( int state ) {
  next = state;
  last_trigger = -1;
  flags &= ~ATM_SLEEP_FLAG;
  return *this;
}

int Machine::state() {
  return current;
}

Machine& Machine::trigger( int evt /* = 0 */ ) {
  int new_state;
  int max_cycle = 8;
  do {
    flags &= ~ATM_SLEEP_FLAG;
    cycle();
    new_state = read_state( state_table + ( current * state_width ) + evt + ATM_ON_EXIT + 1 );
  } while ( --max_cycle && ( new_state == -1 || next_trigger != -1 ) );
  if ( new_state > -1 ) {
    next_trigger = evt;
    flags &= ~ATM_SLEEP_FLAG;
    cycle();  // Pick up the trigger
    flags &= ~ATM_SLEEP_FLAG;
    cycle();  // Process the state change
  }
  return *this;
}

Machine& Machine::setTrace( Stream* stream, swcb_sym_t callback, const char symbols[] ) {
  callback_trace = callback;
  stream_trace = stream;
  _symbols = symbols;
  return *this;
}

uint8_t Machine::sleep( int8_t v /* = 1 */ ) {
  if ( v > -1 ) flags = v ? flags | ATM_SLEEP_FLAG : flags & ~ATM_SLEEP_FLAG;
  return ( flags & ATM_SLEEP_FLAG ) > 0;
}

Machine& Machine::begin( const state_t* tbl, int width ) {
  state_table = tbl;
  state_width = ATM_ON_EXIT + width + 2;
  flags &= ~ATM_SLEEP_FLAG;
  return *this;
}

const char* Machine::mapSymbol( int id, const char map[] ) {
  int cnt = 0;
  int i = 0;
  if ( id == -1 ) return "*NONE*";
  if ( id == 0 ) return map;
  while ( 1 ) {
    if ( map[i] == '\0' && ++cnt == id ) {
      i++;
      break;
    }
    i++;
  }
  return &map[i];
}

// .cycle() Executes one cycle of a state machine
Machine& Machine::cycle( uint32_t time /* = 0 */ ) {
  uint32_t cycle_start = millis();
  do {
    if ( ( flags & ( ATM_SLEEP_FLAG | ATM_CYCLE_FLAG ) ) == 0 ) {
      cycles++;
      flags |= ATM_CYCLE_FLAG;
      if ( next != -1 ) {
        action( ATM_ON_SWITCH );
        if ( callback_trace ) {
          callback_trace( stream_trace, *this, mapSymbol( current == -1 ? current : current + state_width - ATM_ON_EXIT - 1, _symbols ),
                          mapSymbol( next == -1 ? next : next + state_width - ATM_ON_EXIT - 1, _symbols ), mapSymbol( last_trigger, _symbols ),
                          millis() - state_millis, cycles );
        }
        if ( current > -1 ) action( read_state( state_table + ( current * state_width ) + ATM_ON_EXIT ) );
        current = next;
        next = -1;
        state_millis = millis();
        action( read_state( state_table + ( current * state_width ) + ATM_ON_ENTER ) );
        if ( read_state( state_table + ( current * state_width ) + ATM_ON_LOOP ) == ATM_SLEEP ) {
          flags |= ATM_SLEEP_FLAG;
        } else {
          flags &= ~ATM_SLEEP_FLAG;
        }
        cycles = 0;
      }
      state_t i = read_state( state_table + ( current * state_width ) + ATM_ON_LOOP );
      if ( i != -1 ) {
        action( i );
      }
      for ( i = ATM_ON_EXIT + 1; i < state_width; i++ ) {
        state_t next_state = read_state( state_table + ( current * state_width ) + i );
        if ( ( next_state != -1 ) && ( i == state_width - 1 || event( i - ATM_ON_EXIT - 1 ) || next_trigger == i - ATM_ON_EXIT - 1 ) ) {
          state( next_state );
          last_trigger = i - ATM_ON_EXIT - 1;
          next_trigger = -1;
          break;
        }
      }
      flags &= ~ATM_CYCLE_FLAG;
    }
  } while ( millis() - cycle_start < time );
  return *this;
}

// FACTORY

void Appliance::runTiny() {
  Machine* m;
  m = inventory_root;
  while ( m ) {
    if ( ( m->flags & ( ATM_SLEEP_FLAG | ATM_CYCLE_FLAG ) ) == 0 ) m->cycle();
    // Move to the next machine
    m = m->inventory_next;
    // if ( time > 0 && ( millis() - cycle_start ) < time ) break;
  }
}

// .add( machine ) Adds a state machine to the factory by prepending it to the inventory list
Appliance& Appliance::add( Machine& machine ) {
  machine.inventory_next = inventory_root;
  inventory_root = &machine;
  return *this;
}

// .cycle() executes one factory cycle (runs all priority queues a certain number of times)
Appliance& Appliance::run( uint32_t time /* = 0 */ )  // Is it safe to allow recursion here???
{
  uint32_t cycle_start = millis();
  do {
    runTiny();
  } while ( millis() - cycle_start < time );
  return *this;
}
