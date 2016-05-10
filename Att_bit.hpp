
#ifndef Att_bit_h
#define Att_bit_h

#include <Automaton.h>

#define MACHINE TinyMachine
#define TINYMACHINE

class Att_bit : public MACHINE {
 public:
  Att_bit( void ) : MACHINE() {
#ifndef TINYMACHINE      
    class_label = "BIT";
#endif    
  };

  state_t _last_state;
  atm_connector _connection[4];

  enum { OFF, ON };                                       // STATES
  enum { EVT_ON, EVT_OFF, EVT_TOGGLE, EVT_INPUT, ELSE };  // EVENTS
  enum { ACT_ON, ACT_OFF, ACT_INPUT };                    // ACTIONS
  enum { MODE_NULL, MODE_CALLBACK, MODE_MACHINE, MODE_TMACHINE, MODE_FACTORY };

  Att_bit& begin( bool default_state = false );
  Att_bit& onFlip( bool st, atm_cb_t callback, int16_t idx = 0 );
  Att_bit& onFlip( bool st, Machine& machine, state_t event = 0 );
  Att_bit& onFlip( bool st, const char* label, state_t event = 0 );
  Att_bit& onFlip( bool st, TinyMachine& machine, state_t event = 0 );
  Att_bit& onInput( bool st, atm_cb_t callback, int16_t idx = 0 );
  Att_bit& onInput( bool st, Machine& machine, state_t event = 0 );
  Att_bit& onInput( bool st, const char* label, state_t event = 0 );
  Att_bit& onInput( bool st, TinyMachine& machine, state_t event = 0 );
#ifndef TINYMACHINE      
  Att_bit& trace( Stream& stream );
#endif    
  int event( int id );
  void action( int id );
};

#endif
