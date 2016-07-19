#include "Atm_fade.hpp"

#if 0
static const PROGMEM uint8_t etable[256] = {
	0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
	1,  1,  1,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
	2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  3,  3,  3,  3,  3,
	3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  4,
	4,  4,  4,  4,  4,  4,  5,  5,  5,  5,  5,  5,  5,  5,  5,  6,
	6,  6,  6,  6,  6,  6,  6,  7,  7,  7,  7,  7,  7,  8,  8,  8,
	8,  8,  8,  9,  9,  9,  9,  9, 10, 10, 10, 10, 10, 11, 11, 11,
	11, 12, 12, 12, 12, 13, 13, 13, 14, 14, 14, 14, 15, 15, 15, 16,
	16, 16, 17, 17, 18, 18, 18, 19, 19, 20, 20, 21, 21, 21, 22, 22,
	23, 23, 24, 24, 25, 25, 26, 27, 27, 28, 28, 29, 30, 30, 31, 32,
	32, 33, 34, 35, 35, 36, 37, 38, 39, 39, 40, 41, 42, 43, 44, 45,
	46, 47, 48, 49, 50, 51, 52, 53, 55, 56, 57, 58, 59, 61, 62, 63,
	65, 66, 68, 69, 71, 72, 74, 76, 77, 79, 81, 82, 84, 86, 88, 90,
	92, 94, 96, 98,100,102,105,107,109,112,114,117,119,122,124,127,
	130,133,136,139,142,145,148,151,155,158,162,165,169,172,176,180,
	184,188,192,196,201,205,210,214,219,224,229,234,239,244,250,255,
};

#define NUM_ETAB (sizeof(etable)/sizeof(etable[0]))
#define READ_ETAB(i) pgm_read_byte(&etable[i])
#else
#define NUM_ETAB (256)
#define READ_ETAB(i) (i)
#endif

Atm_fade& Atm_fade::begin( int attached_pin ) {
  // clang-format off
  const static state_t state_table[] PROGMEM = {
    /*               ON_ENTER    ON_LOOP       ON_EXIT  EVT_CNT_FADE EVT_TM_FADE   EVT_TM_ON  EVT_TM_OFF   EVT_CNT_RPT  EVT_ON EVT_OFF EVT_BLINK  EVT_TOGGLE  EVT_TOGGLE_BLINK    ELSE  */
    /* IDLE   */      ENT_OFF, ATM_SLEEP,           -1,           -1,         -1,         -1,         -1,           -1,     ON,   IDLE,    START,         ON,            START,     -1,  // LED off
    /* ON     */       ENT_ON, ATM_SLEEP,           -1,           -1,         -1,         -1,         -1,           -1,     ON,   IDLE,    START,       IDLE,             IDLE,     -1,  // LED on
    /* START  */      ENT_OFF,        -1,           -1,           -1,         -1,         -1,         -1,           -1,     ON,   IDLE,    START,       IDLE,             IDLE, STARTU,  // Start fading
    /* STARTU */    ENT_START,        -1,           -1,           -1,         -1,         -1,         UP,           -1,     ON,   IDLE,    START,       IDLE,             IDLE,     -1,  
    /* UP     */       ENT_UP,        -1,           -1,       STARTD,         UP,         -1,         -1,           -1,     ON,   IDLE,    START,       IDLE,             IDLE,     -1,
    /* STARTD */    ENT_START,        -1,           -1,           -1,         -1,       DOWN,         -1,           -1,     ON,   IDLE,    START,       IDLE,             IDLE,     -1,
    /* DOWN   */     ENT_DOWN,        -1,           -1,       REPEAT,       DOWN,         -1,         -1,           -1,     ON,   IDLE,    START,       IDLE,             IDLE,     -1,
    /* REPEAT */   ENT_REPEAT,        -1,           -1,           -1,         -1,         -1,         -1,         IDLE,     ON,   IDLE,    START,       IDLE,             IDLE, STARTU,
  };
  // clang-format on
  Machine::begin( state_table, ELSE );
  pin = attached_pin;
  pinMode( pin, OUTPUT );
  timer_fade.set( 0 );   // Number of ms per slope step (slope duration: rate * 32 ms)
  timer_on.set( 500 );   // Plateau between slopes (in which led is fully on)
  timer_off.set( 500 );  // Pause between slopes (in which led is fully off)
  counter_repeat.set( ATM_COUNTER_OFF );
  repeat_count = ATM_COUNTER_OFF;
  return *this;
}

Atm_fade& Atm_fade::blink( uint32_t duration, uint32_t pause_duration, uint16_t repeat_count /* = ATM_COUNTER_OFF */ ) {
  blink( duration );  // Time in which led is fully on
  pause( pause_duration );
  repeat( repeat_count );
  return *this;
}

Atm_fade& Atm_fade::blink( uint32_t duration ) {
  timer_on.set( duration );  // Plateau between slopes (in which led is fully on)
  return *this;
}

Atm_fade& Atm_fade::pause( uint32_t duration ) {  // Pause between slopes (in which led is fully off)
  timer_off.set( duration ? duration : 1 );       // Make sure off_timer is never 0 (work around)
  return *this;
}

Atm_fade& Atm_fade::fade( unsigned long time_ms, uint8_t pwm_first, uint8_t pwm_last) {
	int fade = time_ms / 2 / (pwm_last - pwm_first + 1);
	if (fade == 0) fade = 1;
	this->fade_first = pwm_first % NUM_ETAB;
	this->fade_last = pwm_last % NUM_ETAB;
	timer_fade.set( time_ms / 2 > 0 ? fade : ATM_TIMER_OFF );  // Number of ms per slope step (slope duration: rate * 32 ms)
	return *this;
}

Atm_fade& Atm_fade::fade( int fade ) {
  this->fade_first = 0;
  this->fade_last = NUM_ETAB - 1;
  this->counter_fade.set(this->fade_last - this->fade_first + 1);
  timer_fade.set( fade >= 0 ? fade : ATM_TIMER_OFF );  // Number of ms per slope step (slope duration: rate * 32 ms)
  return *this;
}

Atm_fade& Atm_fade::repeat( uint16_t repeat ) {
  counter_repeat.set( repeat_count = repeat );
  return *this;
}

int Atm_fade::event( int id ) {
  switch ( id ) {
    case EVT_TM_FADE:
      return timer_fade.expired( this );
    case EVT_TM_ON:
      return timer_on.expired( this );
    case EVT_TM_OFF:
      return timer_off.expired( this );
    case EVT_CNT_FADE:
      return counter_fade.expired();
    case EVT_CNT_RPT:
      return counter_repeat.expired();
  }
  return 0;
}

void Atm_fade::action( int id ) {
  switch ( id ) {
    case ENT_ON:
      analogWrite( pin, 255 );
      return;
    case ENT_REPEAT:
      counter_repeat.decrement();
      return;
    case ENT_OFF:
      counter_repeat.set( repeat_count );
      analogWrite( pin, 0 );
      return;
    case ENT_START:
      if (this->fade_last >= this->fade_first) {
	      this->counter_fade.set(this->fade_last - this->fade_first + 1);
	  } else {
	      this->counter_fade.set(this->fade_first - this->fade_last + 1);
	  }
      return;
    case ENT_UP:
	  if (this->fade_last >= this->fade_first) {
		  //READ_ETAB(this->fade_last - this->fade_first + 1 - counter_fade.value + this->fade_first)
		  analogWrite( pin, READ_ETAB((int)this->fade_last + 1 - counter_fade.value) );
	  } else {
		  //READ_ETAB(this->fade_first - (this->fade_first - this->fade_last + 1 - counter_fade.value) )
		  analogWrite( pin, READ_ETAB((int)this->fade_last - 1 + counter_fade.value) );
	  }
	  counter_fade.decrement();
      return;
    case ENT_DOWN:
	  if (this->fade_last >= this->fade_first) {
		  analogWrite( pin, READ_ETAB(this->fade_first + counter_fade.value - 1) );
	  } else {
		  analogWrite( pin, READ_ETAB(this->fade_first - counter_fade.value + 1) );
	  }
	  counter_fade.decrement();
      return;
  }
}

Atm_fade& Atm_fade::on( void ) {
  trigger( EVT_ON );
  return *this;
}

Atm_fade& Atm_fade::off( void ) {
  trigger( EVT_OFF );
  return *this;
}

Atm_fade& Atm_fade::toggle( void ) {
  trigger( EVT_TOGGLE );
  return *this;
}

Atm_fade& Atm_fade::toggleBlink( void ) {
  trigger( EVT_TOGGLE_BLINK );
  return *this;
}

Atm_fade& Atm_fade::start( void ) {
  trigger( EVT_BLINK );
  return *this;
}

Atm_fade& Atm_fade::trace( Stream& stream ) {
  setTrace( &stream, atm_serial_debug::trace,
            "FADE\0EVT_CNT_FADE\0EVT_TM_FADE\0EVT_TM_ON\0EVT_TM_OFF\0EVT_CNT_RPT\0EVT_ON\0EVT_OFF\0EVT_"
            "BLINK\0ELSE\0IDLE\0ON\0START\0STARTU\0UP\0STARTD\0DOWN\0REPEAT" );
  return *this;
}
