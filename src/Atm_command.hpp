#pragma once

#include <Automaton.h>

class Atm_command : public Machine {
 public:
  enum { IDLE, READCHAR, SEND };
  enum { EVT_INPUT, EVT_EOL, ELSE };

  Atm_command( void ) : Machine(){};
  Atm_command& begin( Stream& stream, char buffer[], int size );
  Atm_command& trace( Stream& stream );
  Atm_command& onCommand( atm_cb_push_t callback, int idx = 0 );
  Atm_command& list( const char* cmds );

  Atm_command& separator( const char sep[] );
  int lookup( int id, const char* cmdlist );
  char* arg( int id );

 private:
  enum { ACT_READCHAR, ACT_SEND };
  atm_connector _oncommand;
  Stream* _stream;
  char* _buffer;
  int _bufsize, _bufptr;
  char _eol, _lastch;
  const char* _separator;
  const char* _commands;

  int event( int id );
  void action( int id );
};
