
#ifndef Att_condition_h
#define Att_condition_h

#include <Automaton.h>

#define MACHINE TinyMachine
#define TINYMACHINE

int const ATM_CONDITION_OPERAND_MAX = 4;

class Att_condition : public MACHINE {
 public:
  Att_condition( void ) : MACHINE() {
#ifndef TINYMACHINE      
    class_label = "CON";
#endif
  };

  state_t _last_state;
  atm_connector _connection[4];
  atm_connector _operand[ATM_CONDITION_OPERAND_MAX];

  enum { OFF, ON };                                       // STATES
  enum { EVT_ON, EVT_OFF, EVT_TOGGLE, EVT_INPUT, ELSE };  // EVENTS
  enum { ACT_ON, ACT_OFF, ACT_INPUT };                    // ACTIONS

  Att_condition& begin( bool default_state = false );
  Att_condition& onFlip( bool st, atm_cb_t callback, int16_t idx = 0 );
  Att_condition& onFlip( bool st, Machine& machine, int16_t event = 0 );
  Att_condition& onFlip( bool st, const char* label, int16_t = 0 );
  Att_condition& onFlip( bool st, TinyMachine& machine, int16_t event = 0 );
  Att_condition& onInput( bool st, atm_cb_t callback, int16_t idx = 0 );
  Att_condition& onInput( bool st, Machine& machine, state_t event = 0 );
  Att_condition& onInput( bool st, const char* label, state_t event = 0 );
  Att_condition& onInput( bool st, TinyMachine& machine, state_t event = 0 );

  Att_condition& IF( Machine& machine, char relOp = '>', state_t match = 0 );
  Att_condition& IF( TinyMachine& machine, char relOp = '>', state_t match = 0 );
  Att_condition& IF( const char* label, char relOp = '>', state_t match = 0 );
  Att_condition& IF( atm_cb_t callback, char relOp = '>', state_t match = 0 );
  Att_condition& AND( Machine& machine, char relOp = '>', state_t match = 0 );
  Att_condition& AND( TinyMachine& machine, char relOp = '>', state_t match = 0 );
  Att_condition& AND( const char* label, char relOp = '>', state_t match = 0 );
  Att_condition& AND( atm_cb_t callback, char relOp = '>', state_t match = 0 );
  Att_condition& OR( Machine& machine, char relOp = '>', state_t match = 0 );
  Att_condition& OR( TinyMachine& machine, char relOp = '>', state_t match = 0 );
  Att_condition& OR( const char* label, char relOp = '>', state_t match = 0 );
  Att_condition& OR( atm_cb_t callback, char relOp = '>', state_t match = 0 );
#ifndef TINYMACHINE      
  Att_condition& trace( Stream& stream );
#endif  

 private:
  const static char relOps[];
  int event( int id );
  void action( int id );
  Att_condition& OP( char logOp, Machine& machine, char relOp, state_t match );
  Att_condition& OP( char logOp, TinyMachine& machine, char relOp, state_t match );
  Att_condition& OP( char logOp, const char* label, char relOp, state_t match );
  Att_condition& OP( char logOp, atm_cb_t callback, char relOp, state_t match );
  atm_connector& getfree( atm_connector& list, int max );

  bool eval_one( uint8_t idx );
  bool eval_all();
};

#endif
