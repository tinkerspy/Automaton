
#ifndef Atm_cmd_h
#define Atm_command_h

#include <Automaton.h>

#undef ATM_CLASSNAME
#define ATM_CLASSNAME Atm_command

class ATM_CLASSNAME;

class ATM_CLASSNAME: public Machine {

  public:
    ATM_CLASSNAME( void ) : Machine() { class_label = "CMD"; };

    void (*_callback)( ATM_CLASSNAME * cmd ) = 0;
    Stream * _stream;
    char * _buffer;
    int _bufsize, _bufptr;
    char _eol, _lastch;
    const char * _sep;

    enum { IDLE, READCHAR, SEND } STATES;
    enum { EVT_INPUT, EVT_EOL, ELSE } EVENTS;
	  enum { ACT_READCHAR, ACT_SEND } ACTIONS;
	
    ATM_CLASSNAME & begin( Stream * stream, char buffer[], int size );
    int event( int id ); 
    void action( int id ); 
    ATM_CLASSNAME & onCmd( void (*callback)( ATM_CLASSNAME * cmd ) ); 
    ATM_CLASSNAME & separator( const char sep[] ); 
    int command( int id, const char * cmdlist );
    char * arg( int id );
};

#endif

