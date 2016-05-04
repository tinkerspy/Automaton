
#ifndef Atm_command_h
#define Atm_command_h

#include <Automaton.h>

class Atm_command: public Machine {

  public:
    Atm_command( void ) : Machine() { class_label = "CMD"; };

    void (*_callback)( int idx ) = 0;
    Stream * _stream;
    char * _buffer;
    int _bufsize, _bufptr;
    char _eol, _lastch;
    const char * _separator;
    const char * _commands;

    enum { IDLE, READCHAR, SEND };
    enum { EVT_INPUT, EVT_EOL, ELSE };
	enum { ACT_READCHAR, ACT_SEND };
	
    Atm_command & begin( Stream & stream, char buffer[], int size );
    Atm_command & trace( Stream & stream );
    int event( int id ); 
    void action( int id ); 
    Atm_command & onCommand(void (*callback)( int idx ), const char * cmds  );
    Atm_command & separator( const char sep[] ); 
    int lookup( int id, const char * cmdlist );
    char * arg( int id );
};

// TinyMachine version

class Att_command: public TinyMachine {

  public:
    Att_command( void ) : TinyMachine() { };

    void (*_callback)( int idx ) = 0;
    Stream * _stream;
    char * _buffer;
    int _bufsize, _bufptr;
    char _eol, _lastch;
    const char * _separator;
    const char * _commands;

    enum { IDLE, READCHAR, SEND };
    enum { EVT_INPUT, EVT_EOL, ELSE };
	enum { ACT_READCHAR, ACT_SEND };
	
    Att_command & begin( Stream & stream, char buffer[], int size );
    int event( int id ); 
    void action( int id ); 
    Att_command & onCommand(void (*callback)( int idx ), const char * cmds  );
    Att_command & separator( const char sep[] ); 
    int lookup( int id, const char * cmdlist );
    char * arg( int id );
};

#endif

