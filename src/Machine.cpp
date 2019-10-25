/*
  Automaton.cpp - Reactive State Machine Framework for Arduino.
  Published under the MIT License (MIT), Copyright (c) 2015-2016, J.P. van der Landen
*/

#include "Automaton.h"

/* The Machine class is a base class for creating and running State Machines
 *
 *********************************************************************************************
 *
 * Machine::state( void ) - Retrieves the current state for the machine
 *
 * (may be overridden by a subclass in which case it may return something else, like a value )
 */

int Machine::state() {
  return current;
}

/*
 * Machine::state( state ) - Sets the next state for the machine
 *
 */

Machine& Machine::state( int state ) {
  next = state;
  last_trigger = -1;
  flags &= ~ATM_SLEEP_FLAG;
  return *this;
}

/*
 * Machine::trigger( evt ) - Triggers an event for the machine
 *
 * The machine is cycled for maximum of 8 times until it is actively listening for the event
 * Then the event is triggered followed by two more cycles to process the event and the
 * following state change.
 *
 */

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

/*
 * Machine::setTrace( stream, callback, symbols ) - Sets up state tracing for the machine
 *
 * Connects a stream object, a callback (atm_serial_debug) and a symbol table (string) to the object
 *
 */

Machine& Machine::setTrace( Stream* stream, swcb_sym_t callback, const char symbols[] ) {
  callback_trace = callback;
  stream_trace = stream;
  this->symbols = symbols;
  return *this;
}

/*
 * Machine::sleep( v ) - Sets or returns the current sleep flag setting
 *
 */

uint8_t Machine::sleep( int8_t v /* = 1 */ ) {
  if ( v > -1 ) flags = v ? flags | ATM_SLEEP_FLAG : flags & ~ATM_SLEEP_FLAG;
  return ( flags & ATM_SLEEP_FLAG ) > 0;
}

/*
 * Machine::begin( state_table, width ) - Initializes the state table and sets the sleep flag
 *
 */

Machine& Machine::begin( const state_t* tbl, int width ) {
  state_table = tbl;
  state_width = ATM_ON_EXIT + width + 2;
  flags &= ~ATM_SLEEP_FLAG;
  automaton.add( *this, false );
  current = -1;
  next = 0;
  next_trigger = -1;
  return *this;
}

/*
 * Machine::onPush( connectors, id, sub, slots, multi, dest, arg ) - Registers a connector destination
 *
 * connectors Connector table
 * id         Connector id
 * sub        Connector sub id (for multi-slot connectors)
 * slots      Number of slots reserved for this connector
 * multi      Register multiple (all) slots in one call
 * dest       Destination: Machine object or callback
 * arg        Argument for machine (event) or callback (idx)
 *
 */

void Machine::onPush( atm_connector connectors[], int id, int sub, int slots, int fill, Machine& machine, int event ) {
  if ( sub == -1 ) {  // auto store
    sub = 0;
    for ( int i = 0; i < slots; i++ ) {
      if ( connectors[id + i].mode() == 0 ) {  // Find a free slot
        sub = i;
      }
    }
  }
  if ( slots > 1 && fill ) {
    for ( int i = 0; i < slots; i++ ) {
      connectors[id + i].set( &machine, event );
    }
  } else {
    connectors[id + sub].set( &machine, event );
  }
}

void Machine::onPush( atm_connector connectors[], int id, int sub, int slots, int fill, atm_cb_push_t callback, int idx ) {
  if ( sub == -1 ) {  // auto store
    sub = 0;
    for ( int i = 0; i < slots; i++ ) {
      if ( connectors[id + i].mode() == 0 ) {  // Find a free slot
        sub = i;
      }
    }
  }
  if ( slots > 1 && fill ) {
    for ( int i = 0; i < slots; i++ ) {
      connectors[id + i].set( callback, idx );
    }
  } else {
    connectors[id + sub].set( callback, idx );
  }
}

/*
 * Machine::push( connectors, id, sub, v, up ) - Pushes an action through the specified connector
 *
 * connectors Connector table
 * id         Connector id
 * sub        Connector sub id (for multi-slot connectors)
 * v          Value to pass to a callback as 'v'
 * up         Value to pass to a callback as 'up'
 *
 */

void Machine::push( atm_connector connectors[], int id, int sub, int v, int up ) {
  if ( ( id & ATM_BROADCAST ) > 0 ) {
    id = id & ~ATM_BROADCAST;
    for ( int i = id; i < sub; i++ ) {
      connectors[id + i].push( v, up );
    }
  } else {
    connectors[id + sub].push( v, up );
  }
}

/*
 * Machine::mapSymbol( id, map ) - Maps a number ( event/state ) to a symbol
 *
 * 0        Machine class name (e.g. LED)
 * 1..ELSE  Event name (e.g. EVT_TIMER)
 * ELSE..   State name (e.g. IDLE)
 *
 */

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

/*
 * Machine::cycle( time ) - Executes one cycle of a State Machine
 *
 * For every state change:
 * - Calls the ON_SWITCH action
 * - Calls the state trace function (if connected)
 * - Calls the previous state's ON_EXIT action
 * - Changes the active state (current) to the new
 * - Calls the new state's ON_ENTER action
 *
 * For every 'normal' cycle:
 * - Executes the ON_LOOP action
 * - Scans the event columns in the current table and calls active events
 *
 * If the 'time' argument is given, loops until that time has passed
 * otherwise executes only one cycle of the machine
 */

Machine& Machine::cycle( uint32_t time /* = 0 */ ) {
  uint32_t cycle_start = millis();
  do {
    if ( ( flags & ( ATM_SLEEP_FLAG | ATM_CYCLE_FLAG ) ) == 0 ) {
      cycles++;
      flags |= ATM_CYCLE_FLAG;
      if ( next != -1 ) {
        action( ATM_ON_SWITCH );
        if ( callback_trace ) {
          callback_trace( stream_trace, *this, symbols, mapSymbol( current == -1 ? current : current + state_width - ATM_ON_EXIT, symbols ),
                          mapSymbol( next == -1 ? next : next + state_width - ATM_ON_EXIT, symbols ),
                          mapSymbol( last_trigger == -1 ? -1 : last_trigger + 1, symbols ), millis() - state_millis, cycles );
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
