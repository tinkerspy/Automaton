#include "Atm_comparator.h"

  
Atm_comparator & Atm_comparator::begin( int attached_pin, int samplerate, triggercb_t cb /* = 0 */ )
{
  const static state_t state_table[] PROGMEM = {
  /*              ON_ENTER    ON_LOOP  ON_EXIT  EVT_TRIGGER EVT_TIMER   ELSE */
  /* IDLE   */          -1,        -1,      -1,           -1,   SAMPLE,    -1,
  /* SAMPLE */  ACT_SAMPLE,        -1,      -1,         SEND,       -1,  IDLE,
  /* SEND   */    ACT_SEND,        -1,      -1,           -1,       -1,  IDLE,
  };
  Machine::begin( state_table, ELSE );
  pin = attached_pin; 
  timer.set( samplerate ); 
  bitmap_sample = 0;
  bitmap_previous = 0;
  callback = cb;
  return *this;          
}

Atm_comparator & Atm_comparator::onUp( Machine & machine, int event /* = 0 */ )
{
  _upmode = MODE_MACHINE;
  _up_machine = &machine;
  _up_machine_event = event;
  return *this;
}

Atm_comparator & Atm_comparator::onUp( TinyMachine & machine, int event /* = 0 */ )
{
  _upmode = MODE_TMACHINE;
  _up_tmachine = &machine;
  _up_tmachine_event = event;
  return *this;
}

Atm_comparator & Atm_comparator::onUp( const char * label, int event /* = 0 */ )
{
  _upmode = MODE_FACTORY;
  _up_label = label;
  _up_label_event = event;
  return *this;
}

Atm_comparator & Atm_comparator::onDown( Machine & machine, int event /* = 0 */ )
{
  _upmode = MODE_MACHINE;
  _down_machine = &machine;
  _down_machine_event = event;
  return *this;
}

Atm_comparator & Atm_comparator::onDown( TinyMachine & machine, int event /* = 0 */ )
{
  _upmode = MODE_TMACHINE;
  _down_tmachine = &machine;
  _down_tmachine_event = event;
  return *this;
}

Atm_comparator & Atm_comparator::onDown( const char * label, int event /* = 0 */ )
{
  _upmode = MODE_FACTORY;
  _down_label = label;
  _down_label_event = event;
  return *this;
}

int Atm_comparator::read_sample() 
{
  return analogRead( pin );
}

int Atm_comparator::_avg() 
{
  uint16_t v = read_sample();
  avg_buf_total = avg_buf_total + v - avg_buf[avg_buf_head];
  avg_buf[avg_buf_head] = v;
  if ( avg_buf_head + 1 >= avg_buf_size ) {
    avg_buf_head = 0;
  } else {
    avg_buf_head++;
  }
  return avg_buf_total / avg_buf_size; 
}

int Atm_comparator::sample() 
{
  return avg_buf_size > 0 ? _avg() : read_sample();
}

Atm_comparator &  Atm_comparator::threshold( uint16_t * v, uint16_t size, bool catchUp /* = false */ ) 
{
   p_threshold = v;
   p_threshold_size = size;
   if ( !catchUp ) {
     v_sample = sample();
     bitmap( v_sample );
     v_previous = v_sample;
     bitmap_previous = bitmap_sample;
   }
   return *this;
}

Atm_comparator &  Atm_comparator::average( uint16_t * v, uint16_t size ) 
{
   avg_buf = v;
   avg_buf_size = size;
   avg_buf_head = 0; 
   avg_buf_total = 0;
   for ( uint16_t i = 0; i < avg_buf_size; i++ ) {
     avg_buf[i] = read_sample();
     avg_buf_total += avg_buf[i];
   }
   return *this;
}

Atm_comparator & Atm_comparator::bitmap( uint16_t v ) 
{
   bitmap_sample = 0;
   for ( uint8_t i = 0; i < p_threshold_size; i++ ) {
     if ( v >= p_threshold[i] ) bitmap_sample |= ( 1 << i );
   }
   bitmap_diff = bitmap_sample ^ bitmap_previous;
   return *this;
}

int Atm_comparator::event( int id ) 
{
  switch ( id ) {
    case EVT_TRIGGER :
      if ( bitmap_diff ) {
        return 1;
      } 
      return 0;
    case EVT_TIMER :
      return timer.expired( this );        
   }
   return 0;
}

void Atm_comparator::action( int id ) 
{
  switch ( id ) {
  	case ACT_SAMPLE :
      v_previous = v_sample;
      bitmap_previous = bitmap_sample;
  	  v_sample = sample();
      bitmap( v_sample );
  	  return;
  	case ACT_SEND :
      if ( v_sample >= v_previous ) {
        for ( uint16_t i = 0; i < p_threshold_size; i++ ) {
          if ( (bitmap_diff >> i ) & 1 ) {
			if ( callback ) {
              (*callback)( v_sample, 1, i, p_threshold[i] );
            }
            if ( _upmode == MODE_MACHINE ) {
				_up_machine->trigger( _up_machine_event );
			}		
            if ( _upmode == MODE_TMACHINE ) {
				_up_tmachine->trigger( _up_tmachine_event );
			}		
            if ( _upmode == MODE_FACTORY ) {
				factory->trigger( _up_label, _up_label_event );
			}		
          }
        }        
      } else {
        for ( int i = p_threshold_size; i >= 0; i-- ) {
          if ( (bitmap_diff >> i ) & 1 ) {
			if ( callback ) {
              (*callback)( v_sample, 0, i, p_threshold[i] ); 
		    }			  
            if ( _downmode == MODE_MACHINE ) {
				_down_machine->trigger( _down_machine_event );
			}		
            if ( _downmode == MODE_TMACHINE ) {
				_down_tmachine->trigger( _down_tmachine_event );
			}		
            if ( _downmode == MODE_FACTORY ) {
				factory->trigger( _down_label, _down_label_event );
			}		
          }
        }
      } 	
      return;
   }
}

Atm_comparator & Atm_comparator::trace( Stream & stream )
{
  setTrace( &stream, atm_serial_debug::trace,
    "EVT_TRIGGER\0EVT_TIMER\0ELSE\0"
    "IDLE\0SAMPLE\0SEND" );

  return *this;
}

// Tiny Machine version
  
Att_comparator & Att_comparator::begin( int attached_pin, int samplerate, triggercb_t cb /* = 0 */ )
{
  const static tiny_state_t state_table[] PROGMEM = {
  /*              ON_ENTER    ON_LOOP  ON_EXIT  EVT_TRIGGER EVT_TIMER   ELSE */
  /* IDLE   */          -1,        -1,      -1,           -1,   SAMPLE,    -1,
  /* SAMPLE */  ACT_SAMPLE,        -1,      -1,         SEND,       -1,  IDLE,
  /* SEND   */    ACT_SEND,        -1,      -1,           -1,       -1,  IDLE,
  };
  TinyMachine::begin( state_table, ELSE );
  pin = attached_pin; 
  timer.set( samplerate ); 
  bitmap_sample = 0;
  bitmap_previous = 0;
  callback = cb;
  return *this;          
}

Att_comparator & Att_comparator::onUp( Machine & machine, int event /* = 0 */ )
{
  _upmode = MODE_MACHINE;
  _up_machine = &machine;
  _up_machine_event = event;
  return *this;
}

Att_comparator & Att_comparator::onUp( TinyMachine & machine, int event /* = 0 */ )
{
  _upmode = MODE_TMACHINE;
  _up_tmachine = &machine;
  _up_tmachine_event = event;
  return *this;
}

Att_comparator & Att_comparator::onDown( Machine & machine, int event /* = 0 */ )
{
  _downmode = MODE_MACHINE;
  _down_machine = &machine;
  _down_machine_event = event;
  return *this;
}

Att_comparator & Att_comparator::onDown( TinyMachine & machine, int event /* = 0 */ )
{
  _downmode = MODE_TMACHINE;
  _down_tmachine = &machine;
  _down_tmachine_event = event;
  return *this;
}

int Att_comparator::read_sample() 
{
  return analogRead( pin );
}

int Att_comparator::_avg() 
{
  uint16_t v = read_sample();
  avg_buf_total = avg_buf_total + v - avg_buf[avg_buf_head];
  avg_buf[avg_buf_head] = v;
  if ( avg_buf_head + 1 >= avg_buf_size ) {
    avg_buf_head = 0;
  } else {
    avg_buf_head++;
  }
  return avg_buf_total / avg_buf_size; 
}

int Att_comparator::sample() 
{
  return avg_buf_size > 0 ? _avg() : read_sample();
}

Att_comparator &  Att_comparator::threshold( uint16_t * v, uint16_t size, bool catchUp /* = false */ ) 
{
   p_threshold = v;
   p_threshold_size = size;
   if ( !catchUp ) {
     v_sample = sample();
     bitmap( v_sample );
     v_previous = v_sample;
     bitmap_previous = bitmap_sample;
   }
   return *this;
}

Att_comparator &  Att_comparator::average( uint16_t * v, uint16_t size ) 
{
   avg_buf = v;
   avg_buf_size = size;
   avg_buf_head = 0; 
   avg_buf_total = 0;
   for ( uint16_t i = 0; i < avg_buf_size; i++ ) {
     avg_buf[i] = read_sample();
     avg_buf_total += avg_buf[i];
   }
   return *this;
}

Att_comparator & Att_comparator::bitmap( uint16_t v ) 
{
   bitmap_sample = 0;
   for ( uint16_t i = 0; i < p_threshold_size; i++ ) {
     if ( v >= p_threshold[i] ) bitmap_sample |= ( 1 << i );
   }
   bitmap_diff = bitmap_sample ^ bitmap_previous;
   return *this;
}

int Att_comparator::event( int id ) 
{
  switch ( id ) {
    case EVT_TRIGGER :
      if ( bitmap_diff ) {
        return 1;
      } 
      return 0;
    case EVT_TIMER :
      return timer.expired( this );        
   }
   return 0;
}

void Att_comparator::action( int id ) 
{
  switch ( id ) {
  	case ACT_SAMPLE :
      v_previous = v_sample;
      bitmap_previous = bitmap_sample;
  	  v_sample = sample();
      bitmap( v_sample );
  	  return;
  	case ACT_SEND :
      if ( v_sample >= v_previous ) {
        for ( uint16_t i = 0; i < p_threshold_size; i++ ) {
          if ( (bitmap_diff >> i ) & 1 ) {
			if ( callback ) {
              (*callback)( v_sample, 1, i, p_threshold[i] );
            }
            if ( _upmode == MODE_MACHINE ) {
				_up_machine->trigger( _up_machine_event );
			}		
            if ( _upmode == MODE_TMACHINE ) {
				_up_tmachine->trigger( _up_tmachine_event );
			}		
          }
        }        
      } else {
        for ( int i = p_threshold_size; i >= 0; i-- ) {
          if ( (bitmap_diff >> i ) & 1 ) {
			if ( callback ) {
              (*callback)( v_sample, 0, i, p_threshold[i] ); 
		    }
            if ( _downmode == MODE_MACHINE ) {
				_down_machine->trigger( _down_machine_event );
			}		
            if ( _downmode == MODE_TMACHINE ) {
				_down_tmachine->trigger( _down_tmachine_event );
			}		
          }
        }
      } 	
      return;
   }
}

