#include <Automaton.h>

class Blink : public Machine {

  public:
    Blink( void ) : Machine() { class_label = "BLNK"; };

    short pin;     
    atm_milli_timer timer;

	enum { LED_ON, LED_OFF } STATES;
	enum { EVT_TIMER, ELSE } EVENTS;
	enum { ACT_ON, ACT_OFF } ACTIONS;
		
    Blink & begin( int attached_pin, int blinkrate )
    {
      const static state_t state_table[] PROGMEM = {
      /*            ON_ENTER    ON_LOOP  ON_EXIT  EVT_TIMER   ELSE */
      /* LED_ON  */   ACT_ON,        -1,      -1,   LED_OFF,    -1,
      /* LED_OFF */  ACT_OFF,        -1,      -1,    LED_ON,    -1 };
      Machine::begin( state_table, ELSE );
      pin = attached_pin; 
      set( timer, blinkrate ); 
      pinMode( pin, OUTPUT ); 
      return *this;          
    }

    int event( int id ) 
    {
      switch ( id ) {
        case EVT_TIMER :
          return expired( timer );        
       }
       return 0;
    }
	
    void action( int id ) 
    {
      switch ( id ) {
        case ACT_ON :
          digitalWrite( pin, HIGH );
          return;
        case ACT_OFF :
          digitalWrite( pin, LOW );
          return;
       }
    }
};

Blink led;

void setup()
{
  led.begin( 3, 200 );
}

void loop()
{
  led.cycle();
}

