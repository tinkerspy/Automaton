#include <Automaton.h>

class Atm_blink : public Machine {

  public:
    Atm_blink( void ) : Machine() {};

    short pin;
    atm_timer_millis timer;

    enum { IDLE, LED_ON, LED_OFF }; // STATES
    enum { EVT_TIMER, EVT_ON, EVT_OFF, ELSE }; // EVENTS
    enum { ENT_ON, ENT_OFF }; // ACTIONS

    Atm_blink & begin( int attached_pin, uint32_t blinkrate ) {
      const static state_t state_table[] PROGMEM = {
        /*            ON_ENTER    ON_LOOP  ON_EXIT  EVT_TIMER  EVT_ON  EVT_OFF  ELSE */
        /* IDLE    */  ENT_OFF,        -1,      -1,        -1, LED_ON,      -1,   -1,
        /* LED_ON  */   ENT_ON,        -1,      -1,   LED_OFF,     -1,    IDLE,   -1,
        /* LED_OFF */  ENT_OFF,        -1,      -1,    LED_ON,     -1,    IDLE,   -1,
      };
      Machine::begin( state_table, ELSE );
      pin = attached_pin;
      timer.set( blinkrate );
      pinMode( pin, OUTPUT );
      return *this;
    }

    int event( int id ) {
      switch ( id ) {
        case EVT_TIMER :
          return timer.expired( this );
      }
      return 0;
    }

    void action( int id ) {
      switch ( id ) {
        case ENT_ON :
          digitalWrite( pin, HIGH );
          return;
        case ENT_OFF :
          digitalWrite( pin, LOW );
          return;
      }
    }
};

Atm_blink led;

void setup() {
  led.begin( 4, 200 );        // Setup a blink machine on pin 4
  led.trigger( led.EVT_ON );  // Turn it on
}

void loop() {
  led.cycle();
}
