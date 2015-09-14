#include <Automaton.h>
#include "Atm_command.h"
	
ATM_CLASSNAME & ATM_CLASSNAME::begin( Stream * stream, char buffer[], int size )
{
  const static state_t state_table[] PROGMEM = {
  /*                  ON_ENTER    ON_LOOP    ON_EXIT  EVT_INPUT   EVT_EOL   ELSE */
  /* IDLE     */            -1,        -1,        -1,  READCHAR,       -1,    -1,
  /* READCHAR */  ACT_READCHAR,        -1,        -1,  READCHAR,     SEND,    -1,
  /* SEND     */      ACT_SEND,        -1,        -1,        -1,       -1,  IDLE,
  };
  table( state_table, ELSE );
  _stream = stream;
  _buffer = buffer;
  _bufsize = size;
  _bufptr = 0;
  _sep = " ";
  _eol = '\n';
  _lastch = '\0';      
  return *this;          
}

ATM_CLASSNAME & ATM_CLASSNAME::onCommand(void (*callback)( int idx ), const char * commands  ) 
{
  _callback = callback;
  _commands = commands;
  return *this;  
}

ATM_CLASSNAME & ATM_CLASSNAME::separator( const char sep[] ) 
{
  _sep = sep;
  return *this;  
}

char * ATM_CLASSNAME::arg( int id ) {

  int cnt = 0;
  int i;
  if ( id == 0 ) return _buffer;
  for ( i = 0; i < _bufptr; i++ ) {
    if ( _buffer[i] == '\0' ) {
      if ( ++cnt == id ) {
        i++;
        break;
      }
    }
  }
  return &_buffer[i];
}

int ATM_CLASSNAME::lookup( int id, const char * cmdlist ) {

  int cnt = 0;
  char * arg = this->arg( id );
  char * a = arg;
  const char * c = cmdlist;
  while ( c[0] != '\0' ) { 
    while ( c[0] != '\0' && toupper( c[0] ) == toupper( a[0] ) ) {
      c++;
      a++;
    }
    if ( a[0] == '\0' ) 
      return cnt;
    if ( c[0] == ' ' ) 
      cnt++;
    a = arg;  
    c++;
  }
  return -1;
}

int ATM_CLASSNAME::event( int id ) 
{
  switch ( id ) {
    case EVT_INPUT :
      return _stream->available();   
    case EVT_EOL :
      return _buffer[_bufptr-1] == _eol || _bufptr >= _bufsize;   
  }
  return 0;
}

void ATM_CLASSNAME::action( int id ) 
{
  switch ( id ) {
  	case ACT_READCHAR :
      if ( _stream->available() ) {
        char ch = _stream->read();
        if ( strchr( _sep, ch ) == NULL ) {
          _buffer[_bufptr++] = ch; 
          _lastch = ch;
        } else {
          if ( _lastch != '\0' )
            _buffer[_bufptr++] = '\0';
          _lastch = '\0';
        }
      }
      return;
    case ACT_SEND :
      _buffer[--_bufptr] = '\0';
      (*_callback)( lookup( 0, _commands ) );
      _lastch = '\0';      
      _bufptr = 0;
  	  return;
   }
}

