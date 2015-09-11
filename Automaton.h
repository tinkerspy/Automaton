/*
  Automaton.h - Library for creating and running Finite State Machines.
  Created by TinkerSpy, July, 2015.
*/
#ifndef Automaton_h
#define Automaton_h

#include "Arduino.h"

#define ATM_MAX_STATES 32767

#if ATM_MAX_STATES < 128
	typedef int8_t state_t;
	#define read_state(addr) (state_t)pgm_read_byte_near(addr)
#else
	typedef int16_t state_t; 
	#define read_state(addr) (state_t)pgm_read_word_near(addr)
#endif

typedef void (*swcb_t)( const char label[], int current, int next, int trigger, uint32_t runtime, uint32_t cycles );

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

class atm_milli_timer {
	public:	uint32_t value;
};

class atm_micro_timer {
	public:	uint32_t value;
};

class atm_counter { 
	public: uint16_t value;
};

class Factory; // pre declare!

class Machine
{
  public:
	virtual int event( int id ) = 0; // Pure virtual methods -> make this an abstract class
	virtual void action( int id ) = 0;
	Machine &state( state_t state);
	state_t state( void );
	Machine & toggle( state_t state1, state_t state2 ); 
	Machine & set(atm_milli_timer &timer, uint32_t v); 
	Machine & set(atm_micro_timer &timer, uint32_t v); 
	Machine & set(atm_counter &counter, uint16_t v); 
	Machine & table( const state_t tbl[], state_t w );
	uint8_t expired(atm_milli_timer timer);
	uint8_t expired(atm_micro_timer timer);
	uint8_t expired(atm_counter &counter);
	uint16_t decrement(atm_counter &counter);
	uint8_t asleep( void );
	Machine & priority( int8_t priority );
	int8_t priority( void );
	uint32_t runtime( void );
	uint32_t micro_runtime( void );
	uint8_t pinChange( uint8_t pin );
	uint8_t pinChange( uint8_t pin, uint8_t hilo );
	Machine & signalWrite( uint8_t id );
	Machine & signalClear( void );
    Machine & signalClear( uint8_t id  );
	Machine & signalMap( uint32_t bitmap );
	int signalRead( uint8_t id );
	int signalPeek( uint8_t id );
	Machine & cycle( void );
	Machine & onSwitch( swcb_t callback );
	Machine & label( const char label[] );
  public:
	int8_t prio;
	int8_t sleep;
	const char * inst_label;
	const char * class_label;
	Machine * inventory_next;
	Machine * priority_next;
    Factory * factory;
  protected:
    state_t next;
	state_t current = -1;
	state_t trigger = -1;
	uint32_t pinstate;
	uint32_t sig, flag;
	uint32_t state_millis, state_micros;
	const state_t* state_table;
	uint8_t width;
	swcb_t switch_callback;
	uint32_t cycles;
};


class Factory 
{
  public:
	Factory & add( Machine & machine );
	Factory & cycle( void );
    Machine * find( const char label[] );
  protected:
	int8_t recalibrate = 1;
	Machine * inventory_root;
	Machine * priority_root[ATM_NO_OF_QUEUES];
  private:
  	void calibrate( void );
	void run( int q );
};

#endif
