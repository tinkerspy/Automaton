
#ifndef Atm_led_h
#define Atm_led_h

#include <Automaton.h>

class Atm_led : public Machine {
 public:
  Atm_led( void ) : Machine() {
    class_label = "LED";
  };

  enum { IDLE, ON, START, BLINK_OFF, DONE, OFF };
  enum { EVT_ON_TIMER, EVT_OFF_TIMER, EVT_COUNTER, EVT_ON, EVT_OFF, EVT_BLINK, EVT_TOGGLE, EVT_TOGGLE_BLINK, ELSE };
  enum { ACT_INIT, ACT_ON, ACT_OFF, ACT_CHAIN };

  Atm_led& begin( int attached_pin, bool activeLow = false );
  Atm_led& blink( uint32_t duration );
  Atm_led& blink( uint32_t duration, uint32_t pause_duration, uint16_t repeat_count = ATM_COUNTER_OFF );
  Atm_led& pause( uint32_t duration );
  Atm_led& fade( int fade );
  Atm_led& repeat( int repeat );
  Atm_led& trace( Stream& stream );
  Atm_led& onFinish( Machine& n, uint8_t event = EVT_BLINK );
  Atm_led& onFinish( Machine& n, Machine& p, uint8_t event = EVT_BLINK );
  Atm_led& onFinish( TinyMachine& n, uint8_t event = EVT_BLINK );
  Atm_led& onFinish( TinyMachine& n, TinyMachine& p, uint8_t event = EVT_BLINK );

 private:
  short pin;
  bool _activeLow;
  int repeat_count;
  atm_timer_millis on_timer, off_timer;
  atm_counter counter;
  union {
    struct {
      Machine* machine_next;
      Machine* machine_previous;
    };
    struct {
      TinyMachine* tmachine_next;
      TinyMachine* tmachine_previous;
    };
  };
  uint8_t machine_event;

  int event( int id );
  void action( int id );
  void chain_next( void );
  void chain_previous( void );
};


#endif
