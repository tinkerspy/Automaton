#include "Atm_command.hpp"

Atm_command& Atm_command::begin( Stream& stream, char buffer[], int size ) {
  // clang-format off
  const static state_t state_table[] PROGMEM = {
    /*                  ON_ENTER    ON_LOOP    ON_EXIT  EVT_INPUT   EVT_EOL   ELSE */
    /* IDLE     */            -1,        -1,        -1,  READCHAR,       -1,    -1,
    /* READCHAR */  ENT_READCHAR,        -1,        -1,  READCHAR,     SEND,    -1,
    /* SEND     */      ENT_SEND,        -1,        -1,        -1,       -1,  IDLE,
  };
  // clang-format on
  Machine::begin( state_table, ELSE );
  this->stream = &stream;
  this->buffer = buffer;
  bufsize = size;
  bufptr = 0;
  separatorChar = " ";
  lastch = '\0';
  return *this;
}

int Atm_command::event( int id ) {
  switch ( id ) {
    case EVT_INPUT:
      return stream->available();
    case EVT_EOL:
      return buffer[bufptr - 1] == '\n' || buffer[bufptr - 1] == '\r' || bufptr >= bufsize;
  }
  return 0;
}

void Atm_command::action( int id ) {
  switch ( id ) {
    case ENT_READCHAR:
      if ( stream->available() ) {
        char ch = stream->read();
        if ( strchr( separatorChar, ch ) == NULL ) {
          buffer[bufptr++] = ch;
          lastch = ch;
        } else {
          if ( lastch != '\0' ) buffer[bufptr++] = '\0';
          lastch = '\0';
        }
      }
      return;
    case ENT_SEND:
      buffer[--bufptr] = '\0';
      oncommand.push( lookup( 0, commands ) );
      lastch = '\0';
      bufptr = 0;
      return;
  }
}

Atm_command& Atm_command::onCommand( atm_cb_push_t callback, int idx /* = 0 */ ) {
  oncommand.set( callback, idx );
  return *this;
}

Atm_command& Atm_command::list( const char* cmds ) {
  commands = cmds;
  return *this;
}

Atm_command& Atm_command::separator( const char sep[] ) {
  separatorChar = sep;
  return *this;
}

char* Atm_command::arg( int id ) {
  int cnt = 0;
  int i;
  if ( id == 0 ) return buffer;
  for ( i = 0; i < bufptr; i++ ) {
    if ( buffer[i] == '\0' ) {
      if ( ++cnt == id ) {
        i++;
        break;
      }
    }
  }
  return &buffer[i];
}

int Atm_command::lookup( int id, const char* cmdlist ) {
  int cnt = 0;
  char* arg = this->arg( id );
  char* a = arg;
  while ( cmdlist[0] != '\0' ) {
    while ( cmdlist[0] != '\0' && toupper( cmdlist[0] ) == toupper( a[0] ) ) {
      cmdlist++;
      a++;
    }
    if ( a[0] == '\0' && ( cmdlist[0] == ' ' || cmdlist[0] == '\0' ) ) return cnt;
    while ( cmdlist[0] != ' ' && cmdlist[0] != '\0' ) cmdlist++;
    cmdlist++;
    a = arg;
    cnt++;
  }
  return -1;
}

Atm_command& Atm_command::trace( Stream& stream ) {
  setTrace( &stream, atm_serial_debug::trace, "COMMAND\0EVT_INPUT\0EVT_EOL\0ELSE\0IDLE\0READCHAR\0SEND" );
  return *this;
}
