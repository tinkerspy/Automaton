/*
  Automaton.cpp - Reactive State Machine Framework for Arduino.
  Published under the MIT License (MIT), Copyright (c) 2015-2016, J.P. van der Landen
*/

#include "Automaton.h"

/*
 * The Appliance class is a simple scheduler for running State Machines
 *
 *********************************************************************************************
 *
 * Appliance::component( machine ) - Adds a State Machine object to the current appliance
 *
 */

Appliance& Appliance::component( Machine& machine ) {
  machine.inventory_next = inventory_root;
  inventory_root = &machine;
  return *this;
}

/*
 * Appliance::run( time ) - Runs the appliance
 *
 * If the 'time' argument is given, loops until that time has passed
 * otherwise executes only one cycle of each machine and exits
 *
 */

Appliance& Appliance::run( uint32_t time /* = 0 */ )  // Is it safe to allow recursion here???
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
