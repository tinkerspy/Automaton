
#ifndef Atm_pulse_h
#define Atm_pulse_h

#include <Automaton.h>

// Detects a pulse (LOW -> HIGH ) on a digital pin with a minimum duration in ms
// On detection another machine is messaged or a callback is fired

typedef void ( *pulsecb_t )( int idx, uint16_t cnt );

class Atm_Pulse_Comm {
 public:
  uint8_t _mode;
  union {
    struct {  // ATM_USR1_FLAG - callback
      void ( *_callback )( int idx, uint16_t cnt );
      int _callback_idx;
      uint16_t _callback_count;
    };
    struct {  // ATM_USR2_FLAG - machine trigger
      Machine* _client_machine;
      state_t _client_machine_event;
    };
    struct {  // ATM_USR3_FLAG - factory trigger
      const char* _client_label;
      state_t _client_label_event;
    };
    struct {  // ATM_USR2_FLAG - Tiny machine trigger
      TinyMachine* _client_tmachine;
      state_t _client_tmachine_event;
    };
  };
};

class Atm_pulse : public Machine {
 public:
  Atm_pulse( void ) : Machine() { class_label = "PULSE"; };

  short pin;
  int state_high, state_low;
  atm_timer_millis timer;
  bool _activeLow;
  Atm_Pulse_Comm _onpulse, _onrelease;

  enum { IDLE, WAIT, PULSE };
  enum { EVT_TIMER, EVT_HIGH, EVT_LOW, ELSE };
  enum { ACT_PULSE, ACT_RELEASE };
  enum { MODE_NULL, MODE_CALLBACK, MODE_MACHINE, MODE_TMACHINE, MODE_FACTORY };

  Atm_pulse& begin( int attached_pin, int minimum_duration = 20, bool activeLow = false, bool pullUp = false );
  Atm_pulse& trace( Stream& stream );
  int event( int id );
  void action( int id );
  Atm_pulse& onPulse( pulsecb_t callback, int idx = 0 );
  Atm_pulse& onPulse( Machine& machine, int event = 0 );
  Atm_pulse& onPulse( TinyMachine& machine, int event = 0 );
  Atm_pulse& onPulse( const char* label, int event = 0 );
  Atm_pulse& onRelease( pulsecb_t callback, int idx = 0 );
  Atm_pulse& onRelease( Machine& machine, int event = 0 );
  Atm_pulse& onRelease( TinyMachine& machine, int event = 0 );
  Atm_pulse& onRelease( const char* label, int event = 0 );
};

// TinyMachine version

class Att_pulse : public TinyMachine {
 public:
  Att_pulse( void ) : TinyMachine(){};

  short pin;
  int state_high, state_low;
  atm_timer_millis timer;
  bool _activeLow;
  Atm_Pulse_Comm _onpulse, _onrelease;

  enum { IDLE, WAIT, PULSE };
  enum { EVT_TIMER, EVT_HIGH, EVT_LOW, ELSE };
  enum { ACT_PULSE, ACT_RELEASE };
  enum { MODE_NULL, MODE_CALLBACK, MODE_MACHINE, MODE_TMACHINE, MODE_FACTORY };

  Att_pulse& begin( int attached_pin, int minimum_duration = 20, bool activeLow = false, bool pullUp = false );
  Att_pulse& trace( Stream& stream );
  int event( int id );
  void action( int id );
  Att_pulse& onPulse( pulsecb_t callback, int idx = 0 );
  Att_pulse& onPulse( Machine& machine, int event = 0 );
  Att_pulse& onPulse( TinyMachine& machine, int event = 0 );
  Att_pulse& onRelease( pulsecb_t callback, int idx = 0 );
  Att_pulse& onRelease( Machine& machine, int event = 0 );
  Att_pulse& onRelease( TinyMachine& machine, int event = 0 );
};

#endif
