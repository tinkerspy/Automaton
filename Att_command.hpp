#pragma once

#include <Automaton.h>

#define TINYMACHINE

#ifdef TINYMACHINE
#define MACHINE TinyMachine
#define FACTORY 0
#define STATE_TYPE tiny_state_t
#else
#define MACHINE Machine
#define FACTORY factory
#define STATE_TYPE state_t
#endif

typedef void ( *atm_command_oncommand_cb_t )( int idx, int cmd );

class Att_command : public MACHINE {
 public:
  Att_command( void ) : MACHINE() {
#ifndef TINYMACHINE      
    class_label = "CMD";
#endif    
  };

  atm_connector _oncommand;
  Stream* _stream;
  char* _buffer;
  int _bufsize, _bufptr;
  char _eol, _lastch;
  const char* _separator;
  const char* _commands;

  enum { IDLE, READCHAR, SEND };
  enum { EVT_INPUT, EVT_EOL, ELSE };
  enum { ACT_READCHAR, ACT_SEND };

  Att_command& begin( Stream& stream, char buffer[], int size );
  Att_command& trace( Stream& stream );
  int event( int id );
  void action( int id );
  Att_command& onCommand( atm_command_oncommand_cb_t callback, int idx  = 0 );
  Att_command& list( const char* cmds );
  
  Att_command& separator( const char sep[] );
  int lookup( int id, const char* cmdlist );
  char* arg( int id );
};

