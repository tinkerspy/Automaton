/*
  Automaton.cpp - Library for creating Finite State Machines.
  Created by TinkerSpy, july, 2015.
*/

#include "Automaton.h"

// .state( state ) Sets the (next) machine state
Machine & Machine::state(state_t state) 
{ 
    next = state; 
    trigger = -1; 
    sleep = 0;
    return *this; 
}

// .state() Returns the current machine state
state_t Machine::state() 
{ 
    return current; 
}

Machine &Machine::toggle( state_t state1, state_t state2 ) 
{ 
    state( current == state1 ? state2 : state1 ); 
    return *this; 
}

Machine & Machine::onSwitch( swcb_t callback ) 
{
    switch_callback = callback;
    return *this;
}

/* Sample callback function

void sw( const char label[], int current, int next, int trigger, uint32_t runtime, uint32_t cycles ) {
  Serial.print( millis() );
  Serial.print( " Switching " );
  Serial.print( label );
  Serial.print( " from state " );
  Serial.print( current );
  Serial.print( " to " );
  Serial.print( next );
  Serial.print( " on trigger " );
  Serial.print( trigger );
  Serial.print( " (" );
  Serial.print( cycles );
  Serial.print( "/" );
  Serial.print( runtime );
  Serial.println( " cycles/ms)" );
}

*/

Machine & Machine::label( const char label[] ) 
{
    inst_label = label;
    return *this;
}

int8_t Machine::priority() 
{ 
    return prio; 
}

Machine & Machine::priority( int8_t priority ) 
{ 
    prio = priority; 
    return *this; 
}

// .asleep() Returns true if the machine is in a sleeping state

uint8_t Machine::asleep() 
{ 
    return sleep; 
}

// .runtime() Returns the number of millis since the machine entered the current state 
uint32_t Machine::runtime() 
{ 
    return millis() - state_millis; 
}

uint32_t Machine::micro_runtime() 
{ 
    return micros() - state_micros; 
}

// .set( timer, value ) Sets a timer to a value
Machine & Machine::set( atm_milli_timer &timer, uint32_t v ) 
{ 
    timer.value = v; 
    return *this; 
}

// .set( timer, value ) Sets a timer to a value
Machine & Machine::set( atm_micro_timer &timer, uint32_t v ) 
{ 
    timer.value = v; 
    return *this; 
}

// .set( counter, value ) Sets a counter to a value
Machine & Machine::set( atm_counter &counter, uint16_t v ) 
{ 
    counter.value = v; 
    return *this; 
}

// .set( state_table, ELSE ) Sets the state_table & table width
Machine & Machine::table( const state_t* tbl, state_t w ) 
{ 
    state_table = tbl;
    width = ATM_ON_EXIT + w + 2;
    prio = ATM_DEFAULT_PRIO;
    return *this; 
}

// .expired( timer) Returns true if the timer argument has expired
uint8_t Machine::expired( atm_milli_timer timer ) 
{ 
    return timer.value == ATM_TIMER_OFF ? 0 : runtime() >= timer.value; 
}

// .expired( timer) Returns true if the timer argument has expired
uint8_t Machine::expired( atm_micro_timer timer ) 
{ 
    return timer.value == ATM_TIMER_OFF ? 0 : micro_runtime() >= timer.value; 
}

// .expired( counter) Returns true if the counter argument (unsigned int) has expired
uint8_t Machine::expired( atm_counter &counter ) 
{ 
    return counter.value == ATM_COUNTER_OFF ? 0 : ( counter.value > 0 ? 0 : 1 ); 
}

// .decrement( counter) 
uint16_t Machine::decrement( atm_counter &counter ) 
{ 
    return counter.value > 0 && counter.value != ATM_COUNTER_OFF ? --counter.value : 0; 
}


int Machine::event( int id ) { 	return 0; }

void Machine::action( int id ) { }

/* 
 * Returns true if pin has changed
 * Clears any change for pin 
 */

unsigned char Machine::pinChange( uint8_t pin ) { 

  unsigned char v = digitalRead( pin ) ? 1 : 0;
  if ( (( pinstate >> pin ) & 1 ) != ( v == 1 ) ) {
    pinstate ^= ( (uint32_t)1 << pin );
    return 1;
  }
  return 0;
}

/* 
 * Returns true if pin has changed to either HIGH or LOW (match hilo)
 * Clears any change for pin 
 */

uint8_t Machine::pinChange( uint8_t pin, uint8_t hilo ) { 

  uint8_t v = digitalRead( pin ) ? 1 : 0;
  if ( (( pinstate >> pin ) & 1 ) != ( v == 1 ) ) {
    pinstate ^= ( (uint32_t)1 << pin );
    return v == hilo ? 1 : 0;
  }
  return 0;
}

Machine & Machine::signalWrite( uint8_t id )
{
    sig |= (uint32_t) 1 << id;
    sleep = 0;
    return *this;
}

int Machine::signalRead( uint8_t id )
{
    if ( ( sig >> id ) & 1 )
    {
        sig ^= (uint32_t) 1 << id;
        return 1;
    }
    return 0;
}

int Machine::signalPeek( uint8_t id )
{
    return ( sig >> id ) & 1;
}

Machine & Machine::signalClear( void )
{
    sig = 0;
    return *this;
}

Machine & Machine::signalMap( uint32_t bitmap )
{
    sig |= bitmap;
    sleep = 0;
    return *this;
}

Machine & Machine::flagSet( int id ) 
{
   flag |= (uint32_t) 1 << id;
   return *this;
}

Machine & Machine::flagClear( int id ) 
{
   flag &= ~(uint32_t) 1 << id;
   return *this;
}

int Machine::flagRead( int id ) 
{
   return ( flag >> id ) & 1;
}

// .cycle() Executes one cycle of a state machine
Machine & Machine::cycle() 
{
    if ( !sleep ) {
        cycles++;
        if ( next != -1 ) {
            action( ATM_ON_SWITCH );
            if ( switch_callback ) 
                switch_callback( inst_label ? inst_label : class_label, current, next, trigger, runtime(), cycles );
            current = next;
            next = -1;
            state_millis = millis();
            state_micros = micros();
            action( read_state( state_table + ( current * width ) + ATM_ON_ENTER ) );
            sleep = read_state( state_table + ( current * width ) + ATM_ON_LOOP ) == ATM_SLEEP;
            cycles = 0;
        }
        state_t i = read_state( state_table + ( current * width ) + ATM_ON_LOOP );
        if ( i != -1 ) { action( i ); }
        for ( i = ATM_ON_EXIT + 1; i < width; i++ ) { 
            if ( ( read_state( state_table + ( current * width ) + i ) != -1 ) && ( event( i - ATM_ON_EXIT - 1 ) || i == width - 1 ) ) {
                action( read_state( state_table + ( current * width ) + ATM_ON_EXIT ) );
                state( read_state( state_table + ( current * width ) + i ) );
                trigger = i - ATM_ON_EXIT - 1;
                return *this;
            }
        }
    }
    return *this;
}

// FACTORY

// .calibrate() Distributes the machines in the inventory to the appropriate priority queues
void Factory::calibrate( void ) 
{
    // Reset all priority queues to empty lists
    for ( int8_t i = 0; i < ATM_NO_OF_QUEUES; i++ ) 
        priority_root[i] = 0;	
    // Walk the inventory list that contains all state machines in this factory
    Machine * m = inventory_root;
    while ( m ) {
        // Prepend every machine to the appropriate priority queue
        if ( m->prio < ATM_NO_OF_QUEUES ) {
            m->priority_next = priority_root[m->prio];
            priority_root[m->prio] = m;
        }
        m = m->inventory_next;
    }		
    recalibrate = 0;
}

// .run( q ) Traverses an individual priority queue and cycles the machines in it once (except for queue 0)
void Factory::run( int q ) 
{
    Machine * m = priority_root[ q ];
    while ( m ) {
        if ( q > 0 && !m->sleep ) m->cycle();
        // Request a recalibrate if the prio field doesn't match the current queue
        if ( m->prio != q ) recalibrate = 1;
        // Move to the next machine
        m = m->priority_next;
    }
}

// .add( machine ) Adds a state machine to the factory by prepending it to the inventory list
Factory & Factory::add( Machine & machine ) 
{	
    machine.inventory_next = inventory_root;
    inventory_root = &machine;
    recalibrate = 1;
    return *this;
}

// .cycle() executes one factory cycle (runs all priority queues a certain number of times)
Factory & Factory::cycle( void ) 
{
    if ( recalibrate ) calibrate();
    run( 1 ); run( 2 );	run( 1 ); run( 2 );
    run( 1 ); run( 3 );	run( 1 ); run( 4 );
    run( 1 ); run( 2 );	run( 1 ); run( 3 );
    run( 1 ); run( 2 );	run( 1 ); run( 0 );
    return *this;
}

