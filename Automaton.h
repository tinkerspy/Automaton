/*
  Automaton.h - Library for creating and running Finite State Machines.
  Published under the MIT License (MIT), Copyright (c) 2015, J.P. van der Landen
*/

/* 
More memory savings!

flags byte: sleep, msg_autoclear, micro_mode
Nog maar 1 soort timer per machine micro( 0 | 1 )
Ditch tiny machine previous
Ditch TinyFactory!

TODO: 
add micros_timer( 0 | 1 ) method
change teensywave to micros timer
REMOVE NAMES FROM ALL ENUMS!!!!
test


*/

#ifndef Automaton_h
#define Automaton_h

#include "Arduino.h"

typedef int16_t state_t; 
typedef int8_t tiny_state_t;

const uint8_t ATM_SLEEP_FLAG = 1;
const uint8_t ATM_MSGAC_FLAG = 2;

#define tiny_read_state(addr) (tiny_state_t)pgm_read_byte_near(addr)
#define read_state(addr) (state_t)pgm_read_word_near(addr)

typedef void (*swcb_sym_t)( const char label[], const char current[], const char next[], const char trigger[], uint32_t runtime, uint32_t cycles );

typedef uint16_t atm_msg_t;

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

class atm_timer {
    public:    
        uint32_t value;
        void set( uint32_t v );
        virtual int expired( BaseMachine * machine ) = 0;
};


class atm_timer_millis: public atm_timer 
{
    public:    
        int expired( BaseMachine * machine );
};


class atm_timer_micros: public atm_timer 
{
    public:    
        int expired( BaseMachine * machine  );
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
  protected:
        uint8_t micros_timer( uint8_t v );
  public:
        uint32_t state_millis, state_micros;
        uint8_t flags = 0;
 
        uint8_t asleep( void );
        virtual int event( int id ) = 0; // Pure virtual methods -> make this an abstract class
        virtual void action( int id ) = 0;
};


class Machine: public BaseMachine
{
    public:
        Machine &state( state_t state);
        state_t state( void );
        int trigger( int evt );
        Machine & toggle( state_t state1, state_t state2 ); 
        Machine & priority( int8_t priority );
        int8_t priority( void );
        int msgClear( uint8_t id_msg ); 
        Machine & msgClear( void ); 
        Machine & msgWrite( uint8_t id_msg ); 
        Machine & msgWrite( uint8_t id_msg, int cnt ); 
        Machine & cycle( void );
        virtual Machine & onSwitch( swcb_sym_t callback, const char sym_s[], const char sym_e[] );
        Machine & label( const char label[] );

        int8_t prio;
        const char * inst_label;
        const char * class_label;
        Machine * inventory_next;
        Machine * priority_next;
        Factory * factory;

    protected:
  
        Machine & begin( const state_t tbl[], int width );
        Machine & msgQueue( atm_msg_t msg[], int width );
        Machine & msgQueue( atm_msg_t msg[], int width, uint8_t autoclear );

        const char * map_symbol( int id, const char map[] );
        uint8_t pinChange( uint8_t pin );
        int msgRead( uint8_t id_msg ); 
        int msgRead( uint8_t id_msg, int cnt ); 
        int msgRead( uint8_t id_msg, int cnt, int clear ); 
        int msgPeek( uint8_t id_msg ); 
        
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
        atm_msg_t * msg_table;
        int msg_width;
};


class TinyMachine: public BaseMachine
{
    public:
        TinyMachine & state( tiny_state_t state);
        tiny_state_t state( void );
        TinyMachine & cycle( void );
        int trigger( int evt );
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
        int msgSend( const char label[], int msg, int cnt );
        int msgSend( const char label[], int msg );
        int msgSendClass( const char label[], int msg, int cnt );
        int msgSendClass( const char label[], int msg );
        Factory & cycle( void );
  private:
        int8_t recalibrate = 1;
        Machine * inventory_root;
        Machine * priority_root[ATM_NO_OF_QUEUES];
        void calibrate( void );
        void run( int q );
};


#endif
