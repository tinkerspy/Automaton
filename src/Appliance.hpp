/*
  Automaton.h - Reactive State Machine Framework for Arduino.
  Published under the MIT License (MIT), Copyright (c) 2015-2016, J.P. van der Landen
*/

#pragma once

#include "Automaton.h"

class Appliance {
 public:
  Appliance& component( Machine& machine, bool force = true );
  Appliance& run( uint32_t time = 0 );

 private:
  Machine* inventory_root;
  void runTiny( void );
};
