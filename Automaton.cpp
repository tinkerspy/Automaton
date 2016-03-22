/*
  Automaton.cpp - Library for creating and running Finite State Machines.
  Published under the MIT License (MIT), Copyright (c) 2015, J.P. van der Landen
*/

#include "Automaton.h"

void atm_timer::set( uint32_t v ) {
    value = v;
}

int atm_timer_millis::expired( BaseMachine * machine ) {
    return value == ATM_TIMER_OFF ? 0 : millis() - machine->state_millis >= value;
}

int atm_timer_micros::expired( BaseMachine * machine ) {
    return value == ATM_TIMER_OFF ? 0 : micros() - machine->state_micros >= value;
}

void atm_counter::set( uint16_t v ) {
    value = v;
}

uint16_t atm_counter::decrement( void ) 
{
    return value > 0 && value != ATM_COUNTER_OFF ? --value : 0; 
}

uint8_t atm_counter::expired() 
{ 
    return value == ATM_COUNTER_OFF ? 0 : ( value > 0 ? 0 : 1 ); 
}




Machine & Machine::state(state_t state) 
{ 
    next = state; 
    last_trigger = -1; 
    sleep = 0;
    if ( msg_autoclear ) msgClear();
    return *this; 
}

state_t Machine::state() 
{ 
    return current; 
}

int Machine::trigger( int evt )
{
    if ( current > -1 ) {
        int new_state = read_state( state_table + ( current * state_width ) + evt + ATM_ON_EXIT + 1 );
        if ( new_state > -1 ) {
	      state( new_state );
          last_trigger = evt;
          return 1;
        }
    }
    return 0;
}

Machine &  Machine::toggle( state_t state1, state_t state2 ) 
{ 
    state( current == state1 ? state2 : state1 ); 
    return *this; 
}

Machine & Machine::onSwitch( swcb_sym_t callback, const char sym_s[], const char sym_e[] ) 
{
    callback_sym = callback;
    sym_states = sym_s;
    sym_events = sym_e;
    return *this;
}

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

uint8_t BaseMachine::asleep() 
{ 
    return sleep; 
}

Machine & Machine::begin( const state_t* tbl, int width ) 
{ 
    state_table = tbl;
    state_width = ATM_ON_EXIT + width + 2;
    prio = ATM_DEFAULT_PRIO;
    if ( !inst_label ) inst_label = class_label;
    return *this; 
}

Machine & Machine::msgQueue( atm_msg_t msg[], int width ) 
{ 
    msg_table = msg;
    msg_width = width;
    return *this; 
}

Machine & Machine::msgQueue( atm_msg_t msg[], int width, uint8_t autoclear ) 
{ 
    msg_table = msg;
    msg_width = width;
    msg_autoclear = autoclear;
    return *this; 
}

unsigned char Machine::pinChange( uint8_t pin ) { 

  unsigned char v = digitalRead( pin ) ? 1 : 0;
  if ( (( pinstate >> pin ) & 1 ) != ( v == 1 ) ) {
    pinstate ^= ( (uint32_t)1 << pin );
    return 1;
  }
  return 0;
}

int Machine::msgRead( uint8_t id_msg ) // Checks msg queue and removes 1 msg
{
  if ( msg_table[id_msg] > 0 ) {
      msg_table[id_msg]--;
      return 1;
  }
  return 0;
}

int Machine::msgRead( uint8_t id_msg, int cnt ) 
{
  if ( msg_table[id_msg] > 0 ) {
    if ( cnt >= msg_table[id_msg] ) {
        msg_table[id_msg] = 0;
    } else {      
        msg_table[id_msg] -= cnt;
    }    
    return 1;
  }
  return 0;
}

int Machine::msgRead( uint8_t id_msg, int cnt, int clear ) 
{
  if ( msg_table[id_msg] > 0 ) {
    if ( cnt >= msg_table[id_msg] ) {
        msg_table[id_msg] = 0;
    } else {      
        msg_table[id_msg] -= cnt;
    }    
	if ( clear ) {
		for ( int i = 0; i < msg_width; i++ ) {
			msg_table[i] = 0;
		}
    }
    return 1;
  }
  return 0;
}

int Machine::msgPeek( uint8_t id_msg ) 
{
  return msg_table[id_msg];
}

int Machine::msgClear( uint8_t id_msg ) // Destructive read (clears queue for this type)
{
  sleep = 0;
  if ( msg_table[id_msg] > 0 ) {
    msg_table[id_msg] = 0;
    return 1;
  }  
  return 0;
}

Machine & Machine::msgClear() 
{
  sleep = 0;
  for ( int i = 0; i < msg_width; i++ ) {
    msg_table[i] = 0;
  }  
  return *this;
}

Machine & Machine::msgWrite( uint8_t id_msg ) 
{
  sleep = 0;
  msg_table[id_msg]++;
  return *this;
}

Machine & Machine::msgWrite( uint8_t id_msg, int cnt ) 
{
  sleep = 0;
  msg_table[id_msg] += cnt;
  return *this;
}

const char * Machine::map_symbol( int id, const char map[] )
{ 
  int cnt = 0;
  int i = 0;
  if ( id == -1 ) return "*NONE*";
  if ( id ==  0 ) return map;
  while ( 1 ) {
    if ( map[i] == '\0' && ++cnt == id ) {
      i++;
      break;
    }
    i++;
  }
  return &map[i];
}

// .cycle() Executes one cycle of a state machine
Machine & Machine::cycle() 
{
    if ( !sleep ) {
        cycles++;
        if ( next != -1 ) {
            action( ATM_ON_SWITCH );
            if ( callback_sym ) {
                callback_sym( inst_label, 
                    map_symbol(      current, sym_states ), 
                    map_symbol(         next, sym_states ), 
                    map_symbol( last_trigger, sym_events ), millis() - state_millis, cycles );                    
            }
            if ( current > -1 )     
		action( read_state( state_table + ( current * state_width ) + ATM_ON_EXIT ) );
            previous = current;
            current = next;
            next = -1;
            state_millis = millis();
            state_micros = micros();
            action( read_state( state_table + ( current * state_width ) + ATM_ON_ENTER ) );
            sleep = read_state( state_table + ( current * state_width ) + ATM_ON_LOOP ) == ATM_SLEEP;
            cycles = 0;
        }
        state_t i = read_state( state_table + ( current * state_width ) + ATM_ON_LOOP );
        if ( i != -1 ) { action( i ); }
        for ( i = ATM_ON_EXIT + 1; i < state_width; i++ ) { 
            if ( ( read_state( state_table + ( current * state_width ) + i ) != -1 ) && ( i == state_width - 1 || event( i - ATM_ON_EXIT - 1 ) ) ) {
                state( read_state( state_table + ( current * state_width ) + i ) );
                last_trigger = i - ATM_ON_EXIT - 1;
                return *this;
            }
        }
    }
    return *this;
}

// TINY MACHINE


TinyMachine & TinyMachine::state(tiny_state_t state)
{
    next = state;
    sleep = 0;
    return *this;
}

tiny_state_t TinyMachine::state()
{
    return current;
}

int TinyMachine::trigger( int evt )
{
    if ( current > -1 ) {
        int new_state = tiny_read_state( state_table + ( current * state_width ) + evt + ATM_ON_EXIT + 1 );
        if ( new_state > -1 ) {
          state( new_state );
          return 1;
        }
    }
    return 0;
}

TinyMachine & TinyMachine::begin( const tiny_state_t* tbl, int width )
{
    state_table = tbl;
    state_width = ATM_ON_EXIT + width + 2;
    return *this;
}

// .cycle() Executes one cycle of a state machine
TinyMachine & TinyMachine::cycle()
{
    if ( !sleep ) {
        if ( next != -1 ) {
            action( ATM_ON_SWITCH );
            if ( current > -1 )
                action( tiny_read_state( state_table + ( current * state_width ) + ATM_ON_EXIT ) );
            previous = current;
            current = next;
            next = -1;
            state_millis = millis();
            state_micros = micros();
            action( tiny_read_state( state_table + ( current * state_width ) + ATM_ON_ENTER ) );
            sleep = tiny_read_state( state_table + ( current * state_width ) + ATM_ON_LOOP ) == ATM_SLEEP;
        }
        tiny_state_t i = tiny_read_state( state_table + ( current * state_width ) + ATM_ON_LOOP );
        if ( i != -1 ) { action( i ); }
        for ( i = ATM_ON_EXIT + 1; i < state_width; i++ ) {
            if ( ( tiny_read_state( state_table + ( current * state_width ) + i ) != -1 ) && ( i == state_width - 1 || event( i - ATM_ON_EXIT - 1 ) ) ) {
                state( tiny_read_state( state_table + ( current * state_width ) + i ) );
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
    machine.factory = this;
    recalibrate = 1;
    machine.cycle();
    return *this;
}

// .find() Search the factory inventory for a machine by instance label
Machine * Factory::find( const char label[] )
{
    Machine * m = inventory_root;
    while ( m ) {
        if ( strcmp( label, m->inst_label ) == 0 ) {
            return m;
        }
        m = m->inventory_next;
    }
    return 0;        
}

    
int Factory::msgSend( const char label[], int msg, int cnt )
{
    int r = 0;
    Machine * m = inventory_root;
    while ( m ) {
        if ( strcmp( label, m->inst_label ) == 0 ) {
            m->msgWrite( msg, cnt );
            r++;
        }
        m = m->inventory_next;
    }
    return r;        
}    

int Factory::msgSend( const char label[], int msg )
{
    msgSend( label, msg, 1 );
}

int Factory::msgSendClass( const char label[], int msg, int cnt )
{
    int r = 0;
    Machine * m = inventory_root;
    while ( m ) {
        if ( strcmp( label, m->class_label ) == 0 ) {
            m->msgWrite( msg, cnt );
            r++;
        }
        m = m->inventory_next;
    }
    return r;        
}    

int Factory::msgSendClass( const char label[], int msg )
{
    msgSendClass( label, msg, 1 );
}

// .cycle() executes one factory cycle (runs all priority queues a certain number of times)
Factory & Factory::cycle( void ) 
{
    if ( recalibrate ) calibrate();
    run( 1 ); run( 2 );	run( 1 ); run( 2 );
    run( 1 ); run( 3 );	run( 1 ); run( 4 );
    run( 1 ); run( 2 );	run( 1 ); run( 3 );
    run( 1 ); run( 2 );	run( 1 ); run( 0 );
    return  *this;
}


// TINYFACTORY - A factory for TinyMachines

// .add( machine ) Adds a state machine to the factory by prepending it to the inventory list
TinyFactory & TinyFactory::add( TinyMachine & machine ) 
{	
    machine.inventory_next = inventory_root;
    inventory_root = &machine;
    machine.cycle();
    return *this;
}


// .cycle() executes the factory cycle 
TinyFactory &  TinyFactory::cycle( void ) 
{
    TinyMachine * m = inventory_root;
    while ( m ) {
        if ( !m->sleep ) m->cycle();
        // Move to the next machine
        m = m->inventory_next;
    }
    return *this; 
}



