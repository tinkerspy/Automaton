/*
  Automaton.cpp - Reactive State Machine Framework for Arduino.
  Published under the MIT License (MIT), Copyright (c) 2015-2016, J.P. van der Landen
*/

#include "Automaton.h"

Automaton automaton; // The global automaton machine container/scheduler

/*
 * The Automaton class is a simple scheduler for running State Machines
 *
 *********************************************************************************************
 *
 * Automaton::add( machine ) - Adds a State Machine object to the container
 *
 */

Automaton& Automaton::add( Machine& machine, bool force /* = true */ ) {
  if ( machine.inventory_next == 0 || force ) {
    machine.inventory_next = inventory_root;
    inventory_root = &machine;
  }
  return *this;
}

/*
 * Automaton::run( time ) - Runs the appliance
 *
 * If the 'time' argument is given, loops until that time has passed
 * otherwise executes only one cycle of each machine and exits
 *
 */

Automaton& Automaton::run( uint32_t time /* = 0 */ )  // Is it safe to allow recursion here???
{
  Machine* m;
  uint32_t cycle_start = millis();
  do {
    m = inventory_root;
    while ( m ) {
      if ( ( m->flags & ( ATM_SLEEP_FLAG | ATM_CYCLE_FLAG ) ) == 0 ) m->cycle();
      // Move to the next machine
      m = m->inventory_next;
      // if ( time > 0 && ( millis() - cycle_start ) < time ) break;
    }
  } while ( millis() - cycle_start < time );
  return *this;
}

Appliance& Appliance::component( Machine& machine ) {
  Automaton::add( machine );
  return *this;
}

Appliance& Appliance::run( uint32_t time /* = 0 */ ) {
  Automaton::run( time );
  return *this;
}


Factory& Factory::add( Machine& machine ) {
  Automaton::add( machine );
  return *this;
}

Factory& Factory::cycle( uint32_t time /* = 0 */ ) {
  Automaton::run( time );
  return *this;
}
