/*
  Automaton.cpp - Reactive State Machine Framework for Arduino.
  Published under the MIT License (MIT), Copyright (c) 2015-2016, J.P. van der Landen
*/

#include "Automaton.h"

/*
 * The atm_connector class facilitates creating push and pull connections between
 * State Machines.
 *
 *********************************************************************************************
 *
 * push( v, up, overrideCallback ) - Calls a machine's trigger method or a callback
 *
 * Will return false if a callback is configured while the overrideCallback arg was specified
 */

bool atm_connector::push( int v /* = 0 */, int up /* = 0 */, bool overrideCallback /* = false */ ) {
  switch ( mode_flags & B00000111 ) {
    case MODE_PUSHCB:
      if ( overrideCallback ) {
        return false;
      } else {
        if ( push_callback ) {
          ( *push_callback )( callback_idx, v, up );
        }
      }
      return true;
    case MODE_MACHINE:
      if ( machine != 0 ) {
        machine->trigger( event );
      }
      return true;
  }
  return true;
}

/*
 * pull( v, up, def_value ) - Calls a machine's state method or a callback
 *
 */

int atm_connector::pull( int v /* = 0 */, int up /* = 0 */, bool def_value /* = false */ ) {
  switch ( mode_flags & B00000111 ) {
    case MODE_PULLCB:
      return ( *pull_callback )( callback_idx );
    case MODE_MACHINE:
      return machine->state();
  }
  return def_value;
}

/*
 * logOp() Returns the logical operator part of the mode_flags byte
 *
 */

int8_t atm_connector::logOp( void ) {
  return ( mode_flags & B00011000 ) >> 3;
}

/*
 * logOp() Returns the relational operator part of the mode_flags byte
 *
 */

int8_t atm_connector::relOp( void ) {
  return ( mode_flags & B11100000 ) >> 5;
}

/*
 * set( callback, idx, logOp, relOp ) - Configures a connector object as a push callback
 *
 */

void atm_connector::set( atm_cb_push_t callback, int idx, int8_t logOp /* = 0 */, int8_t relOp /* = 0 */ ) {
  mode_flags = MODE_PUSHCB | ( logOp << 3 ) | ( relOp << 5 );
  push_callback = callback;
  callback_idx = idx;
}

/*
 * set( callback, idx, logOp, relOp ) - Configures a connector object as a pull callback
 *
 */

void atm_connector::set( atm_cb_pull_t callback, int idx, int8_t logOp /* = 0 */, int8_t relOp /* = 0 */ ) {
  mode_flags = MODE_PULLCB | ( logOp << 3 ) | ( relOp << 5 );
  pull_callback = callback;
  callback_idx = idx;
}

/*
 * set( callback, idx, logOp, relOp ) - Configures a connector object as a machine connector (calls trigger)
 *
 */

void atm_connector::set( Machine* m, int evt, int8_t logOp /* = 0 */, int8_t relOp /* = 0 */ ) {
  mode_flags = MODE_MACHINE | ( logOp << 3 ) | ( relOp << 5 );
  machine = m;
  event = evt;
}

/*
 * mode() - Returns the mode part of the mode_flags byte
 *
 */

int8_t atm_connector::mode( void ) {
  return mode_flags & B00000111;
}
