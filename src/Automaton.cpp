/*
  Automaton.cpp - Reactive State Machine Framework for Arduino.
  Published under the MIT License (MIT), Copyright (c) 2015-2016, J.P. van der Landen
*/

#include "Automaton.h"

Automaton automaton;  // The global automaton machine container/scheduler

/*
 * The Automaton class is a simple scheduler for running State Machines
 *
 *********************************************************************************************
 *
 * Automaton::add( machine ) - Adds a State Machine object to the container
 *
 */

Automaton& Automaton::add( Machine& machine, bool force /* = true */ ) {
  if ( ( machine.flags & ATM_SLINK_FLAG ) == 0 || force ) {
    machine.inventory_next = inventory_root;
    inventory_root = &machine;
    machine.flags |= ATM_SLINK_FLAG;
  }
  return *this;
}

/*
 * Automaton::run() - Runs the appliance
 *
 * executes one cycle of each machine and exits
 *
 */

Automaton& Automaton::run( void ) {
  Machine* m;
  m = inventory_root;
  while ( m ) {
    if ( ( m->flags & ( ATM_SLEEP_FLAG | ATM_CYCLE_FLAG ) ) == 0 ) m->cycle();
    // Move to the next machine
    m = m->inventory_next;
  }
  return *this;
}

void Automaton::printf(Stream & stream, const char *format, ...) {
  static char line[80];
  va_list args;
  va_start(args, format);
  uint8_t len = vsnprintf(line, sizeof(line), format, args);
  va_end(args);
  for (char *p = &line[0]; *p; p++) {
      if (*p == '\n') {
          stream.write('\r');
      }
      stream.write(*p);
  }
  if (len >= sizeof(line))
      stream.write('$');
}

Automaton& Automaton::delay( uint32_t time ) {
  uint32_t cycle_start = millis();
  do {
    run();
  } while ( millis() - cycle_start < time );
  return *this;
}

Appliance& Appliance::component( Machine& machine ) {
  Automaton::add( machine );
  return *this;
}

Appliance& Appliance::run( uint32_t time /* = 0 */ ) {
  Automaton::delay( time );
  return *this;
}

Factory& Factory::add( Machine& machine ) {
  Automaton::add( machine );
  return *this;
}

Factory& Factory::cycle( uint32_t time /* = 0 */ ) {
  Automaton::delay( time );
  return *this;
}
