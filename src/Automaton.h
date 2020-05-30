/*
  Automaton.h - Reactive State Machine Framework for Arduino.
  Published under the MIT License (MIT), Copyright (c) 2015-2016, J.P. van der Landen
*/

#pragma once

#include "Arduino.h"

/*
  fixing bug cause by new ardiuino api version
  See https://github.com/arduino/ArduinoCore-API/issues/25
*/
#if (ARDUINO_API_VERSION >= 10000)
  
  inline void digitalWrite(unsigned pin, bool status) { 
    digitalWrite(pin, status ? 1 : 0); 
  };
#endif

typedef int8_t state_t;

const uint8_t ATM_SLEEP_FLAG = 0b00000001;
const uint8_t ATM_CYCLE_FLAG = 0b00000010;
const uint8_t ATM_SLINK_FLAG = 0b00000100;
const uint8_t ATM_USR1_FLAG = 0b00010000;
const uint8_t ATM_USR2_FLAG = 0b00100000;
const uint8_t ATM_USR3_FLAG = 0b01000000;
const uint8_t ATM_USR4_FLAG = 0b10000000;
const uint8_t ATM_USR_FLAGS = 0b11110000;
const uint8_t ATM_BROADCAST = 0b10000000;

#define read_state( addr ) ( state_t ) pgm_read_byte_near( addr )

#define B2INT( hibyte, lobyte ) ((int)( hibyte << 8 | lobyte ))
#define B4INT( byte4, byte3, byte2, byte1) ((uint32_t)( (uint32_t)byte4 << 24 | (uint32_t)byte3 << 16 | (uint32_t)byte2 << 8 | (uint32_t)byte1 ))

class Machine;
class Automaton;

extern Automaton automaton;

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

class Automaton {
 public:
  Automaton& add( Machine& machine, bool force = true );
  Automaton& delay( uint32_t time );
  Automaton& run( void );

 private:
  Machine* inventory_root;
  void runTiny( void );
};

// For backward compatibility with 'the old way': Appliance app/app.component()/app.run()

class Appliance : Automaton {
 public:
  Appliance& component( Machine& machine );
  Appliance& run( uint32_t time = 0 );
};

class Factory : Automaton {
 public:
  Factory& add( Machine& machine );
  Factory& cycle( uint32_t time = 0 );
};

// Support classes

#include <atm_connector.hpp>
#include <atm_timer_millis.hpp>
#include <atm_counter.hpp>
#include <atm_serial_debug.hpp>

// Main framework classes

#include <Machine.hpp>

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
