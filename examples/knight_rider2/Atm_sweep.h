#pragma once

#include <Automaton.h>

class Atm_sweep: public Machine {

  public:
    Atm_sweep( void ) : Machine() {};

    enum { IDLE, U0, U1, U2, U3, U4, U5, D4, D3, D2, D1, D0 };
    enum { EVT_TIMER, EVT_START, EVT_STOP, EVT_TOGGLE, ELSE };
    enum { ENT_L0, ENT_L1, ENT_L2, ENT_L3, ENT_L4, ENT_L5, ENT_OFF  };

    Atm_sweep & begin( int p1, int p2, int p3, int p4, int p5, int p6 );
    Atm_sweep & speed( uint32_t v );
    Atm_sweep & trace( Stream & stream );
    
  private:  
    short pin[6];
    atm_timer_millis timer;

    int event( int id );
    void action( int id );
};
