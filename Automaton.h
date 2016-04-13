/*
  Automaton.h - Library for creating and running Finite State Machines.
  Published under the MIT License (MIT), Copyright (c) 2015, J.P. van der Landen
*/


#ifndef Automaton_h
#define Automaton_h

#include "Arduino.h"

typedef int16_t state_t; 
typedef int8_t tiny_state_t;

const uint8_t ATM_SLEEP_FLAG = 1;
const uint8_t ATM_USR1_FLAG = 16;
const uint8_t ATM_USR2_FLAG = 32;
const uint8_t ATM_USR3_FLAG = 64;
const uint8_t ATM_USR4_FLAG = 128;

#define tiny_read_state(addr) (tiny_state_t)pgm_read_byte_near(addr)
#define read_state(addr) (state_t)pgm_read_word_near(addr)

typedef void (*swcb_sym_t)( const char label[], const char current[], const char next[], const char trigger[], uint32_t runtime, uint32_t cycles );

const int8_t ATM_NO_OF_QUEUES = 5; // queues 0, 1, 2, 3, 4
const int8_t ATM_DEFAULT_PRIO = 1;

const state_t ATM_NOP = -1;
const state_t ATM_SLEEP = -2;
const state_t ATM_ON_SWITCH = -3;
const state_t ATM_ON_ENTER = 0;
const state_t ATM_ON_LOOP = 1;
const state_t ATM_ON_EXIT = 2;

const uint32_t ATM_TIMER_OFF = 0xffffffff; // This timer value never expires
const uint16_t ATM_COUNTER_OFF = 0xffff; // This counter value never expires

class Factory;
class Machine;
class BaseMachine;

class atm_serial_debug {
  public: 
  static void onSwitch( const char label[], const char current[], const char next[], 
        const char trigger[], uint32_t runtime, uint32_t cycles ) {
    Serial.print( millis() );
    Serial.print( " Switch " );
    Serial.print( label );
    Serial.print( " from " );
    Serial.print( current );
    Serial.print( " to " );
    Serial.print( next );
    Serial.print( " on " );
    Serial.print( trigger );
    Serial.print( " (" );
    Serial.print( cycles );
    Serial.print( " cycles in " );
    Serial.print( runtime );
    Serial.println( " ms)" );
  }
};

class atm_timer_millis 
{
    public:    
        uint32_t value;
        void set( uint32_t v );
        int expired( BaseMachine * machine );
};

class atm_timer_micros 
{
    public:    
        uint32_t value;
        void set( uint32_t v );
        int expired( Machine * machine );
};

class atm_counter { 
    public: 
        uint16_t value;
        void set( uint16_t v );
        uint8_t expired( void );
        uint16_t decrement( void );
};

class BaseMachine
{
  public:
        uint32_t state_millis;
        uint8_t flags = ATM_SLEEP_FLAG;
 
        uint8_t sleep( int8_t v = -1 );
        virtual int event( int id ) = 0; // Pure virtual methods -> make this an abstract class
        virtual void action( int id ) = 0;
};

class Machine: public BaseMachine
{
    public:
        Machine &state( state_t state);
        state_t state( void );
        int trigger( int evt );
        Machine & priority( int8_t priority );
        int8_t priority( void );
        Machine & cycle( uint32_t time = 0 );
        virtual Machine & onSwitch( swcb_sym_t callback, const char sym_s[], const char sym_e[] );
        Machine & label( const char label[] );

        int8_t prio;
        uint32_t state_micros;
        const char * inst_label;
        const char * class_label;
        Machine * inventory_next;
        Machine * priority_next;
        Factory * factory;

    protected:
  
        Machine & begin( const state_t tbl[], int width );
        const char * mapSymbol( int id, const char map[] );
        
        const state_t* state_table;
        state_t next;
        state_t current = -1;
        state_t last_trigger = -1;
        state_t previous = -1;
        uint32_t pinstate;
        const char* sym_states;
        const char* sym_events;
        uint8_t state_width;
        swcb_sym_t callback_sym;
        uint32_t cycles;
};


class TinyMachine: public BaseMachine
{
    public:
        TinyMachine & state( tiny_state_t state);
        tiny_state_t state( void );
        TinyMachine & cycle( uint32_t time = 0 );
        int trigger( int evt );
         TinyMachine * inventory_next;
    protected:
        TinyMachine & begin( const tiny_state_t tbl[], int width );
        const tiny_state_t* state_table;
        tiny_state_t next = 0;
        tiny_state_t current = -1;
        uint8_t state_width;
};


class Factory 
{
  public:
        Factory & add( Machine & machine );
        Machine * find( const char label[] );
        int trigger( const char label[], int event );
        int state( const char label[], int state );
        Factory & cycle( uint32_t time = 0 ); 
  private:
        int8_t recalibrate = 1;
        Machine * inventory_root;
        Machine * priority_root[ATM_NO_OF_QUEUES];
        void calibrate( void );
        void run( int q );
};

class TinyFactory 
{
  public:
        TinyFactory & add( TinyMachine & machine );
        TinyMachine * inventory_root;
        TinyFactory & cycle( uint32_t time = 0 );
};

#endif
