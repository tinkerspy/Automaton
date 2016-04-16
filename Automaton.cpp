/*
  Automaton.cpp - Library for creating and running Finite State Machines.
  Published under the MIT License (MIT), Copyright (c) 2015, J.P. van der Landen
*/

#include "Automaton.h"

void atm_timer_millis::set( uint32_t v ) {
    value = v;
}

void atm_timer_micros::set( uint32_t v ) {
    value = v;
}

int atm_timer_millis::expired( BaseMachine * machine ) {
    return value == ATM_TIMER_OFF ? 0 : millis() - machine->state_millis >= value;
}

int atm_timer_micros::expired( Machine * machine ) {
    return value == ATM_TIMER_OFF ? 0 : micros() - machine->state_micros >= value;
}

void atm_counter::set( uint16_t v ) 
{
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
    flags &= ~ATM_SLEEP_FLAG;
    return *this; 
}

state_t Machine::state() 
{ 
    return current; 
}

int Machine::trigger( int evt )
{
    cycle();
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

Machine & Machine::trace( Stream * stream, swcb_sym_t callback, const char sym_s[], const char sym_e[] ) 
{
    callback_trace = callback;
    stream_trace = stream;
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

uint8_t BaseMachine::sleep( int8_t v /* = 1 */ ) 
{
    if ( v > -1 ) 
        flags = v ? flags | ATM_SLEEP_FLAG : flags & ~ATM_SLEEP_FLAG;
    return ( flags & ATM_SLEEP_FLAG ) > 0;
}

Machine & Machine::begin( const state_t* tbl, int width ) 
{ 
    state_table = tbl;
    state_width = ATM_ON_EXIT + width + 2;
    prio = ATM_DEFAULT_PRIO;
    if ( !inst_label ) inst_label = class_label;
    flags &= ~ATM_SLEEP_FLAG;
    return *this; 
}

const char * Machine::mapSymbol( int id, const char map[] )
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
Machine & Machine::cycle( uint32_t time /* = 0 */ ) 
{
    uint32_t cycle_start = millis();
    do {
        if ( ( flags & ( ATM_SLEEP_FLAG | ATM_CALLBACK_FLAG ) ) == 0 ) {
            cycles++;
            if ( next != -1 ) {
                action( ATM_ON_SWITCH );
                if ( callback_trace ) {
                    callback_trace( stream_trace, inst_label, 
                        mapSymbol(      current, sym_states ), 
                        mapSymbol(         next, sym_states ), 
                        mapSymbol( last_trigger, sym_events ), millis() - state_millis, cycles ); 
                }
                if ( current > -1 )     
                        action( read_state( state_table + ( current * state_width ) + ATM_ON_EXIT ) );
                current = next;
                next = -1;
                state_millis = millis();
                state_micros = micros();
                action( read_state( state_table + ( current * state_width ) + ATM_ON_ENTER ) );
                if ( read_state( state_table + ( current * state_width ) + ATM_ON_LOOP ) == ATM_SLEEP ) {
                      flags |= ATM_SLEEP_FLAG;
                } else {
                      flags &= ~ATM_SLEEP_FLAG;
                }
                cycles = 0;
            }
            state_t i = read_state( state_table + ( current * state_width ) + ATM_ON_LOOP );
            if ( i != -1 ) { action( i ); }
            for ( i = ATM_ON_EXIT + 1; i < state_width; i++ ) { 
                if ( ( read_state( state_table + ( current * state_width ) + i ) != -1 ) && ( i == state_width - 1 || event( i - ATM_ON_EXIT - 1 ) ) ) {
                    state( read_state( state_table + ( current * state_width ) + i ) );
                    last_trigger = i - ATM_ON_EXIT - 1;
                    break;
                }
            }
        }
    } while ( millis() - cycle_start < time );
    return *this;
}

// TINY MACHINE


TinyMachine & TinyMachine::state( tiny_state_t state )
{
    next = state;
    flags &= ~ATM_SLEEP_FLAG;
    return *this;
}

tiny_state_t TinyMachine::state()
{
    return current;
}

int TinyMachine::trigger( int evt )
{
    cycle();
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
    flags &= ~ATM_SLEEP_FLAG;
    return *this;
}

// .cycle() Executes one cycle of a state machine
TinyMachine & TinyMachine::cycle( uint32_t time /* = 0 */ )
{
    uint32_t cycle_start = millis();
    do {
        if ( ( flags & ( ATM_SLEEP_FLAG | ATM_CALLBACK_FLAG ) ) == 0 ) {
            if ( next != -1 ) {
                action( ATM_ON_SWITCH );
                if ( current > -1 )
                    action( tiny_read_state( state_table + ( current * state_width ) + ATM_ON_EXIT ) );
                current = next;
                next = -1;
                state_millis = millis();
                action( tiny_read_state( state_table + ( current * state_width ) + ATM_ON_ENTER ) );
                if ( read_state( state_table + ( current * state_width ) + ATM_ON_LOOP ) == ATM_SLEEP ) {
                      flags |= ATM_SLEEP_FLAG;
                } else {
                      flags &= ~ATM_SLEEP_FLAG;
                }
            }
            tiny_state_t i = tiny_read_state( state_table + ( current * state_width ) + ATM_ON_LOOP );
            if ( i != -1 ) { action( i ); }
            for ( i = ATM_ON_EXIT + 1; i < state_width; i++ ) {
                if ( ( tiny_read_state( state_table + ( current * state_width ) + i ) != -1 ) && ( i == state_width - 1 || event( i - ATM_ON_EXIT - 1 ) ) ) {
                    state( tiny_read_state( state_table + ( current * state_width ) + i ) );
                    break;
                }
            }
        }
    } while ( millis() - cycle_start < time );
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
        if ( q > 0 && ( m->flags & ( ATM_SLEEP_FLAG | ATM_CALLBACK_FLAG ) ) == 0 ) m->cycle();
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
    
int Factory::trigger( const char label[], int event )
{
    int r = 0;
    int l = 255;
    Machine * m = inventory_root;
    if ( label[strlen( label ) - 1 ] == '*' ) {
        l = strlen( label ) - 1;
    }
    if ( label[0] == '.' ) {
        l--;
        label++;
        while ( m ) {
            if ( strncmp( label, m->class_label, l ) == 0 ) {
                m->trigger( event );
                r++;
            }
            m = m->inventory_next;
        }
    } else {
        while ( m ) {
            if ( strncmp( label, m->inst_label, l ) == 0 ) {
                m->trigger( event );
                r++;
            }
            m = m->inventory_next;
        }
    }
    return r;        
}    

int Factory::state( const char label[], int state )
{
    int r = 0;
    int l = 255;
    Machine * m = inventory_root;
    if ( label[strlen( label ) - 1 ] == '*' ) {
        l = strlen( label ) - 1;
    }
    if ( label[0] == '.' ) {
        l--;
        label++;
        while ( m ) {
            if ( strncmp( label, m->class_label, l ) == 0 ) {
                m->state( state );
                r++;
            }
            m = m->inventory_next;
        }
    } else {
        while ( m ) {
            if ( strncmp( label, m->inst_label, l ) == 0 ) {
                m->state( state );
                r++;
            }
            m = m->inventory_next;
        }
    }
    return r;        
}    

int Factory::state( const char label[] )
{
    int r = 0;
    int l = 255;
    Machine * m = inventory_root;
    if ( label[strlen( label ) - 1 ] == '*' ) {
        l = strlen( label ) - 1;
    }
    if ( label[0] == '.' ) {
        l--;
        label++;
        while ( m ) {
            if ( strncmp( label, m->class_label, l ) == 0 ) {
                r += m->state();
            }
            m = m->inventory_next;
        }
    } else {
        while ( m ) {
            if ( strncmp( label, m->inst_label, l ) == 0 ) {
                r += m->state();
            }
            m = m->inventory_next;
        }
    }
    return r;        
}    

// .cycle() executes one factory cycle (runs all priority queues a certain number of times)
Factory & Factory::cycle( uint32_t time /* = 0 */ ) 
{
    uint32_t cycle_start = millis();
    if ( recalibrate ) calibrate();
    do {
        run( 1 ); run( 2 );	run( 1 ); run( 2 );
        run( 1 ); run( 3 );	run( 1 ); run( 4 );
        run( 1 ); run( 2 );	run( 1 ); run( 3 );
        run( 1 ); run( 2 );	run( 1 ); run( 0 );
    } while ( millis() - cycle_start < time );
    return  *this;
}

// TINYFACTORY - A factory for TinyMachines

// .add( machine ) Adds a state machine to the factory by prepending it to the inventory list
TinyFactory & TinyFactory::add( TinyMachine & machine ) 
{	
    machine.inventory_next = inventory_root;
    inventory_root = &machine;
    return *this;
}


// .cycle() executes the factory cycle 
TinyFactory &  TinyFactory::cycle( uint32_t time /* = 0 */ ) 
{
    TinyMachine * m;
    uint32_t cycle_start = millis();
    do {
        m = inventory_root;
        while ( m ) {
            if ( ( m->flags & ( ATM_SLEEP_FLAG | ATM_CALLBACK_FLAG ) ) == 0 ) m->cycle();
            // Move to the next machine
            m = m->inventory_next;
            //if ( time > 0 && ( millis() - cycle_start ) < time ) break;
        }
    } while ( ( millis() - cycle_start ) < time );
    return *this; 
}


