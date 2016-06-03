/*
 *   Automaton - Reactive State Machine Framework for Arduino.
 *   Published under the MIT License (MIT), Copyright (c) 2015-2016, J.P. van der Landen
 */

#pragma once

#include "Arduino.h"

class atm_timer_millis {
 public:
  uint32_t value;
  void set( uint32_t v );
  int expired( Machine* machine );
};
