
#ifndef Atm_condition_h
#define Atm_condition_h

#include <Automaton.h>

int const ATM_CONDITION_OPERAND_MAX = 4;

class Atm_condition : public Machine {
 public:
  Atm_condition( void ) : Machine() {
    class_label = "CON";
  };

  state_t _last_state;
  atm_connector _connection[4];
  atm_connector _operand[ATM_CONDITION_OPERAND_MAX];

  enum { OFF, ON };                                       // STATES
  enum { EVT_ON, EVT_OFF, EVT_TOGGLE, EVT_INPUT, ELSE };  // EVENTS
  enum { ACT_ON, ACT_OFF, ACT_INPUT };                    // ACTIONS

  Atm_condition& begin( bool default_state = false );
  Atm_condition& onFlip( bool st, atm_cb_t callback, int16_t idx = 0 );
  Atm_condition& onFlip( bool st, Machine& machine, int16_t event = 0 );
  Atm_condition& onFlip( bool st, const char* label, int16_t = 0 );
  Atm_condition& onFlip( bool st, TinyMachine& machine, int16_t event = 0 );
  Atm_condition& onInput( bool st, atm_cb_t callback, int16_t idx = 0 );
  Atm_condition& onInput( bool st, Machine& machine, state_t event = 0 );
  Atm_condition& onInput( bool st, const char* label, state_t event = 0 );
  Atm_condition& onInput( bool st, TinyMachine& machine, state_t event = 0 );

  Atm_condition& IF( Machine& machine, char relOp = '>', state_t match = 0 );
  Atm_condition& IF( TinyMachine& machine, char relOp = '>', state_t match = 0 );
  Atm_condition& IF( const char* label, char relOp = '>', state_t match = 0 );
  Atm_condition& IF( atm_cb_t callback, char relOp = '>', state_t match = 0 );
  Atm_condition& AND( Machine& machine, char relOp = '>', state_t match = 0 );
  Atm_condition& AND( TinyMachine& machine, char relOp = '>', state_t match = 0 );
  Atm_condition& AND( const char* label, char relOp = '>', state_t match = 0 );
  Atm_condition& AND( atm_cb_t callback, char relOp = '>', state_t match = 0 );
  Atm_condition& OR( Machine& machine, char relOp = '>', state_t match = 0 );
  Atm_condition& OR( TinyMachine& machine, char relOp = '>', state_t match = 0 );
  Atm_condition& OR( const char* label, char relOp = '>', state_t match = 0 );
  Atm_condition& OR( atm_cb_t callback, char relOp = '>', state_t match = 0 );
  Atm_condition& trace( Stream& stream );

 private:
  const static char relOps[];
  int event( int id );
  void action( int id );
  Atm_condition& OP( char logOp, Machine& machine, char relOp, state_t match );
  Atm_condition& OP( char logOp, TinyMachine& machine, char relOp, state_t match );
  Atm_condition& OP( char logOp, const char* label, char relOp, state_t match );
  Atm_condition& OP( char logOp, atm_cb_t callback, char relOp, state_t match );
  atm_connector& getfree( atm_connector& list, int max );

  bool eval_one( uint8_t idx );
  bool eval_all();
};

// Tiny Machine version

#endif
