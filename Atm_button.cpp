#include "Atm_button.h"

// Add option for button press callback (for reading i2c buttons etc)

Atm_button & Atm_button::begin( int attached_pin )
{
	const static state_t state_table[] PROGMEM = {
	/* Standard Mode: press/repeat */
		/*                  ON_ENTER  ON_LOOP       ON_EXIT  EVT_LMODE  EVT_TIMER  EVT_DELAY  EVT_REPEAT EVT_PRESS  EVT_RELEASE  EVT_COUNTER   EVT_AUTO  ELSE */
		/* IDLE     */            -1,      -1,           -1,     LIDLE,        -1,        -1,         -1,     WAIT,          -1,          -1,      AUTO,   -1,
		/* WAIT     */            -1,      -1,           -1,        -1,   PRESSED,        -1,         -1,       -1,        IDLE,          -1,        -1,    -1,
		/* PRESSED  */     ACT_PRESS,      -1,           -1,        -1,        -1,    REPEAT,         -1,       -1,     RELEASE,          -1,        -1,    -1,
		/* REPEAT   */     ACT_PRESS,      -1,           -1,        -1,        -1,        -1,     REPEAT,       -1,     RELEASE,          -1,        -1,    -1,
		/* RELEASE  */   ACT_RELEASE,      -1,           -1,        -1,        -1,        -1,         -1,       -1,          -1,          -1,        -1,  IDLE,
	/* Long Press Mode: press/long press */	
		/* LIDLE    */            -1,      -1,           -1,        -1,        -1,        -1,         -1,    LWAIT,          -1,          -1,        -1,    -1,
		/* LWAIT    */    ACT_LSTART,      -1,           -1,        -1,  LPRESSED,        -1,         -1,       -1,       LIDLE,          -1,        -1,    -1,
		/* LPRESSED */    ACT_LCOUNT,      -1,           -1,        -1,        -1,  LPRESSED,         -1,       -1,    LRELEASE,    WRELEASE,        -1,    -1,
		/* LRELEASE */  ACT_LRELEASE,      -1, ACT_WRELEASE,        -1,        -1,        -1,         -1,       -1,          -1,          -1,        -1, LIDLE,
		/* WRELEASE */  ACT_LRELEASE,      -1, ACT_WRELEASE,        -1,        -1,        -1,         -1,       -1,       LIDLE,          -1,        -1,    -1,
		/* AUTO     */      ACT_AUTO,      -1,           -1,        -1,        -1,        -1,         -1,       -1,          -1,          -1,        -1,  IDLE,
	};	
	Machine::begin( state_table, ELSE );
	_pin = attached_pin;
    _counter_longpress.set( 0 );	
	_timer_debounce.set( _DEBOUNCE );
	_timer_delay.set( ATM_TIMER_OFF );
	_timer_repeat.set( ATM_TIMER_OFF );
	_timer_auto.set( ATM_TIMER_OFF );
	pinMode( _pin, INPUT_PULLUP );
	return *this;
}

Atm_button & Atm_button::onPress( presscb_t callback, int idx /* = 0 */ ) 
{
  _callback = callback;
  _callback_idx = idx;
  _press_count = 0;
  flags &= ~ATM_USR_FLAGS;
  flags |= ATM_USR1_FLAG;
  return *this;  
}

Atm_button & Atm_button::onPress( Machine & machine, int event /* = 0 */ ) 
{
  _client_machine = &machine;
  _client_machine_event = event;
  flags &= ~ATM_USR_FLAGS;
  flags |= ATM_USR2_FLAG;
  return *this;  
}

Atm_button & Atm_button::onPress( const char * label, int event /* = 0 */ )
{
  _client_label = label;
  _client_label_event = event;
  flags &= ~ATM_USR_FLAGS;
  flags |= ATM_USR3_FLAG;
  return *this;  
}

Atm_button & Atm_button::onPress( TinyMachine & machine, int event /* = 0 */ ) 
{
  _client_tmachine = &machine;
  _client_tmachine_event = event;
  flags &= ~ATM_USR_FLAGS;
  flags |= ATM_USR4_FLAG;
  return *this;  
}

Atm_button & Atm_button::debounce( int delay ) {
	
	_timer_debounce.set( delay );
	return *this;
}

Atm_button & Atm_button::longPress( int max, int delay ) {
	
	_longpress_max = max;
    _counter_longpress.set( _longpress_max );
	_timer_delay.set( delay );
	return *this;
}

Atm_button & Atm_button::repeat( int delay /* = 500 */ , int speed /* = 50 */ ) {
	
	_timer_delay.set( delay );
	_timer_repeat.set( speed );	
	return *this;
}

Atm_button & Atm_button::autoPress( int delay, int press /* = 1 */ ) {
	
    _auto_press = press;
	_timer_auto.set( delay );    
	return *this;
}

int Atm_button::event( int id ) 
{
  switch ( id ) {
	case EVT_LMODE :
	  return _counter_longpress.value > 0;        
	case EVT_TIMER :
	  return _timer_debounce.expired( this );
	case EVT_DELAY :
	  return _timer_delay.expired( this );        
	case EVT_REPEAT :
	  return _timer_repeat.expired( this );        
	case EVT_AUTO :
	  return _timer_auto.expired( this );        
	case EVT_PRESS :
	  return !digitalRead( _pin );        
	case EVT_RELEASE :
	  return digitalRead( _pin );        
	case EVT_COUNTER :
	  return _counter_longpress.expired();        
  }
  return 0;
}

void Atm_button::cb( int press, int idx ) {

    if ( press > 0 ) _press_count++;
    if ( ( flags & ATM_USR1_FLAG ) > 0 ) {
      (*_callback)( press, idx, _press_count );
	}
}

void Atm_button::action( int id ) 
{
  switch ( id ) {
	case ACT_PRESS :
	case ACT_AUTO :
      cb( id == ACT_AUTO ? _auto_press : 1, _callback_idx );
      if ( ( flags & ATM_USR2_FLAG ) > 0 ) {
          _client_machine->trigger( _client_machine_event );                
      }
      if ( ( flags & ATM_USR3_FLAG ) > 0 && factory ) {
          factory->trigger( _client_label, _client_label_event );                
      }
      if ( ( flags & ATM_USR4_FLAG ) > 0 ) {
          _client_tmachine->trigger( _client_tmachine_event );                
      }
	  return;
	case ACT_RELEASE :
      cb( 0, _callback_idx );
	  return;
	case ACT_LSTART :
	  _counter_longpress.set( _longpress_max );
	  return;
	case ACT_LCOUNT :
	  _counter_longpress.decrement();
	  cb( ( _longpress_max - _counter_longpress.value ) * -1, _callback_idx );	  
	  return;
	case ACT_LRELEASE :
	  cb( _longpress_max - _counter_longpress.value, _callback_idx );
	  return;
	case ACT_WRELEASE :
	  cb( 0, _callback_idx );
	  return;
  }
}

Atm_button & Atm_button::trace( Stream & stream ) {

  setTrace( &stream, atm_serial_debug::trace,
    "EVT_LMODE\0EVT_TIMER\0EVT_DELAY\0EVT_REPEAT\0EVT_PRESS\0EVT_RELEASE\0EVT_COUNTER\0EVT_AUTO\0ELSE\0IDLE\0WAIT\0PRESSED\0REPEAT\0RELEASE\0LIDLE\0LWAIT\0LPRESSED\0LRELEASE\0WRELEASE\0AUTO" ); 
  return *this;
}

// ATT_BUTTON


Att_button & Att_button::begin( int attached_pin )
{
	const static tiny_state_t state_table[] PROGMEM = {
	/* Standard Mode: press/repeat */
		/*                  ON_ENTER  ON_LOOP       ON_EXIT  EVT_LMODE  EVT_TIMER  EVT_DELAY  EVT_REPEAT EVT_PRESS  EVT_RELEASE  EVT_COUNTER   EVT_AUTO  ELSE */
		/* IDLE     */            -1,      -1,           -1,     LIDLE,        -1,        -1,         -1,     WAIT,          -1,          -1,      AUTO,   -1,
		/* WAIT     */            -1,      -1,           -1,        -1,   PRESSED,        -1,         -1,       -1,        IDLE,          -1,        -1,    -1,
		/* PRESSED  */     ACT_PRESS,      -1,           -1,        -1,        -1,    REPEAT,         -1,       -1,     RELEASE,          -1,        -1,    -1,
		/* REPEAT   */     ACT_PRESS,      -1,           -1,        -1,        -1,        -1,     REPEAT,       -1,     RELEASE,          -1,        -1,    -1,
		/* RELEASE  */   ACT_RELEASE,      -1,           -1,        -1,        -1,        -1,         -1,       -1,          -1,          -1,        -1,  IDLE,
	/* Long Press Mode: press/long press */	
		/* LIDLE    */            -1,      -1,           -1,        -1,        -1,        -1,         -1,    LWAIT,          -1,          -1,        -1,    -1,
		/* LWAIT    */    ACT_LSTART,      -1,           -1,        -1,  LPRESSED,        -1,         -1,       -1,       LIDLE,          -1,        -1,    -1,
		/* LPRESSED */    ACT_LCOUNT,      -1,           -1,        -1,        -1,  LPRESSED,         -1,       -1,    LRELEASE,    WRELEASE,        -1,    -1,
		/* LRELEASE */  ACT_LRELEASE,      -1, ACT_WRELEASE,        -1,        -1,        -1,         -1,       -1,          -1,          -1,        -1, LIDLE,
		/* WRELEASE */  ACT_LRELEASE,      -1, ACT_WRELEASE,        -1,        -1,        -1,         -1,       -1,       LIDLE,          -1,        -1,    -1,
		/* AUTO     */      ACT_AUTO,      -1,           -1,        -1,        -1,        -1,         -1,       -1,          -1,          -1,        -1,  IDLE,
	};
	TinyMachine::begin( state_table, ELSE );
	pin = attached_pin;
    counter_longpress.set( 0 );	
	timer_debounce.set( DEBOUNCE );
	timer_delay.set( ATM_TIMER_OFF );
	timer_repeat.set( ATM_TIMER_OFF );
	timer_auto.set( ATM_TIMER_OFF );
	pinMode( pin, INPUT_PULLUP );
	return *this;
}


Att_button & Att_button::onPress( presscb_t callback, int idx /* = 0 */ ) 
{
  _callback = callback;
  callback_idx = idx;
  _press_count = 0;
  flags &= ~ATM_USR_FLAGS;
  flags |= ATM_USR1_FLAG;
  return *this;  
}

Att_button & Att_button::onPress( Machine & machine, int event /* = 0 */ ) 
{
  _client_machine = &machine;
  _client_machine_event = event;
  flags &= ~ATM_USR_FLAGS;
  flags |= ATM_USR2_FLAG;
  return *this;  
}

Att_button & Att_button::onPress( TinyMachine & machine, int event /* = 0 */ ) 
{
  _client_tmachine = &machine;
  _client_tmachine_event = event;
  flags &= ~ATM_USR_FLAGS;
  flags |= ATM_USR4_FLAG;
  return *this;  
}

Att_button & Att_button::debounce( int delay ) {
	
	timer_debounce.set( delay );
	return *this;
}

Att_button & Att_button::longPress( int max, int delay ) {
	
	longpress_max = max;
    counter_longpress.set( longpress_max );
	timer_delay.set( delay );
	return *this;
}

Att_button & Att_button::repeat( int delay /* = 500 */ , int speed /* = 50 */ ) 
{	
	timer_delay.set( delay );
	timer_repeat.set( speed );	
	return *this;
}

Att_button & Att_button::autoPress( int delay, int press /* = 1 */ ) {
	
    _auto_press = press;
	timer_auto.set( delay );    
	return *this;
}

int Att_button::event( int id ) 
{
  switch ( id ) {
	case EVT_LMODE :
	  return counter_longpress.value > 0;        
	case EVT_TIMER :
	  return timer_debounce.expired( this );
	case EVT_DELAY :
	  return timer_delay.expired( this );        
	case EVT_REPEAT :
	  return timer_repeat.expired( this );        
	case EVT_AUTO :
	  return timer_auto.expired( this );        
	case EVT_PRESS :
	  return !digitalRead( pin );        
	case EVT_RELEASE :
	  return digitalRead( pin );        
	case EVT_COUNTER :
	  return counter_longpress.expired();        
  }
  return 0;
}

void Att_button::cb( int press, int idx ) {

    if ( press > 0 ) _press_count++;
    if ( ( flags & ATM_USR1_FLAG ) > 0 ) {
      (*_callback)( press, idx, _press_count );
	}
}

void Att_button::action( int id ) 
{
  switch ( id ) {
	case ACT_PRESS :
	case ACT_AUTO :
      cb( id == ACT_AUTO ? _auto_press : 1, callback_idx );
      if ( ( flags & ATM_USR2_FLAG ) > 0 ) {
          _client_machine->trigger( _client_machine_event );                
      }
      if ( ( flags & ATM_USR4_FLAG ) > 0 ) {
          _client_tmachine->trigger( _client_tmachine_event );                
      }
	  return;
	case ACT_RELEASE :
      cb( 0, callback_idx );
 	  return;
	case ACT_LSTART :
	  counter_longpress.set( longpress_max );
	  return;
	case ACT_LCOUNT :
	  counter_longpress.decrement();
	  cb( ( longpress_max - counter_longpress.value ) * -1, callback_idx );	  
	  return;
	case ACT_LRELEASE :
	  cb( longpress_max - counter_longpress.value, callback_idx );
	  return;
	case ACT_WRELEASE :
	  cb( 0, callback_idx );
	  return;
  }
}


