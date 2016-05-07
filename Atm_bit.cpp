#include "Atm_bit.hpp"
  
Atm_bit & Atm_bit::begin( bool default_state /* = false */ ) {
  const static state_t state_table[] PROGMEM = {
    /*              ON_ENTER    ON_LOOP  ON_EXIT  EVT_ON  EVT_OFF  EVT_TOGGLE EVT_INPUT ELSE */
    /* OFF     */    ACT_OFF, ATM_SLEEP,      -1,     ON,      -1,         ON,      OFF,  -1,
    /* ON      */     ACT_ON, ATM_SLEEP,      -1,     -1,     OFF,        OFF,       ON,  -1,
  };
  Machine::begin( state_table, ELSE );
  _last_state = -1;
  state( default_state ? ON : OFF );
  cycle();
  return *this;          
}

Atm_bit & Atm_bit::onFlip( bool st, stepcb_t callback )
{
  short idx = st ? 0 : 1;
  _comm[idx]._mode = MODE_CALLBACK;
  _comm[idx]._callback = callback;
  return *this;
}

Atm_bit & Atm_bit::onFlip( bool st, Machine & machine, state_t event /* = 0 */ )
{
  short idx = st ? 0 : 1;
  _comm[idx]._mode = MODE_MACHINE;
  _comm[idx]._client_machine = &machine;
  _comm[idx]._client_machine_event = event;
  return *this;
}

Atm_bit & Atm_bit::onFlip( bool st, const char * label, state_t event /* = 0 */ )
{
  short idx = st ? 0 : 1;
  _comm[idx]._mode = MODE_FACTORY;
  _comm[idx]._client_label = label;
  _comm[idx]._client_label_event = event;
  return *this;
}

Atm_bit & Atm_bit::onFlip( bool st, TinyMachine & machine, state_t event /* = 0 */ )
{
  short idx = st ? 0 : 1;
  _comm[idx]._mode = MODE_TMACHINE;
  _comm[idx]._client_tmachine = &machine;
  _comm[idx]._client_tmachine_event = event;
  return *this;
}

Atm_bit & Atm_bit::onInput( bool st, stepcb_t callback )
{
  short idx = st ? 2 : 3;
  _comm[idx]._mode = MODE_CALLBACK;
  _comm[idx]._callback = callback;
  return *this;
}

Atm_bit & Atm_bit::onInput( bool st, Machine & machine, state_t event /* = 0 */ )
{
  short idx = st ? 2 : 3;
  _comm[idx]._mode = MODE_MACHINE;
  _comm[idx]._client_machine = &machine;
  _comm[idx]._client_machine_event = event;
  return *this;
}

Atm_bit & Atm_bit::onInput( bool st, const char * label, state_t event /* = 0 */ )
{
  short idx = st ? 2 : 3;
  _comm[idx]._mode = MODE_FACTORY;
  _comm[idx]._client_label = label;
  _comm[idx]._client_label_event = event;
  return *this;
}

Atm_bit & Atm_bit::onInput( bool st, TinyMachine & machine, state_t event /* = 0 */ )
{
  short idx = st ? 2 : 3;
  _comm[idx]._mode = MODE_TMACHINE;
  _comm[idx]._client_tmachine = &machine;
  _comm[idx]._client_tmachine_event = event;
  return *this;
}

int Atm_bit::event( int id ) {
   switch (id ) {
   }
   return 0;
}

void Atm_bit::comm( AtmBitComm & c, state_t st ) {

  switch ( c._mode ) {
    case MODE_CALLBACK :
      (*c._callback)( st );
      return;
    case MODE_MACHINE :
      c._client_machine->trigger( c._client_machine_event );
      return;
    case MODE_TMACHINE :
      c._client_tmachine->trigger( c._client_tmachine_event );
      return;
    case MODE_FACTORY:
      factory->trigger( c._client_label, c._client_label_event );
      return;
  }
}

void Atm_bit::action( int id ) {
  switch ( id ) {
    case ACT_OFF :
      if ( _last_state != -1 )
         comm( _comm[ _last_state == current ? 3 : 1 ], current );
      _last_state = current;
      return;
    case ACT_ON :
      if ( _last_state != -1 )
        comm( _comm[ _last_state == current ? 2 : 0 ], current );
      _last_state = current;
      return;
   }
}

Atm_bit & Atm_bit::trace( Stream & stream ) {
  Machine::setTrace( &stream, atm_serial_debug::trace,
    "EVT_ON\0EVT_OFF\0EVT_TOGGLE\0EVT_INPUT\0ELSE\0OFF\0ON\0INPUTM" );
  return *this;
}



