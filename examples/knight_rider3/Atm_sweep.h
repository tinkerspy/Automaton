
#ifndef Atm_sweep_h
#define Atm_sweep_h

#include <Automaton.h>

class Atm_sweep: public Machine {

  public:
    Atm_sweep( void ) : Machine() {
      class_label = "SWP";
    };

    enum { IDLE, U0, U1, U2, U3, U4, U5, D4, D3, D2, D1, D0 };
    enum { EVT_TIMER, EVT_START, EVT_STOP, EVT_TOGGLE, ELSE };
    enum { ACT_L0, ACT_L1, ACT_L2, ACT_L3, ACT_L4, ACT_L5, ACT_OFF  };

    Atm_sweep & begin( int p1, int p2, int p3, int p4, int p5, int p6 );
    Atm_sweep & speed( uint32_t v );
    Atm_sweep & trace( Stream & stream );
    
  private:  
    short pin[6];
    atm_timer_millis timer;

    int event( int id );
    void action( int id );
};

#endif
