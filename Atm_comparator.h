
#ifndef Atm_comparator_h
#define Atm_comparator_h

#include <Automaton.h>

typedef void (*triggercb_t)( int v, int up, int idx_threshold, int v_threshold );

class Atm_comparator: public Machine {

  public:
    Atm_comparator( void ) : Machine() { class_label = "CMP"; };

    short pin;     
    atm_timer_millis timer;
    int v_sample, v_threshold, v_previous;
    uint64_t bitmap_sample, bitmap_previous, bitmap_diff; 
    uint16_t * p_threshold; // Max 64 values
    uint16_t p_threshold_size;
    
    uint16_t * avg_buf;
    uint16_t avg_buf_size;
    uint16_t avg_buf_head;
    uint32_t avg_buf_total;
    
    void (*callback)( int v, int up, int idx_threshold, int v_threshold ) = 0;
    
    enum { IDLE, SAMPLE, SEND } STATES;
    enum { EVT_TRIGGER, EVT_TIMER, ELSE } EVENTS;
	  enum { ACT_SAMPLE, ACT_SEND } ACTIONS;
	
    Atm_comparator & begin( int attached_pin, int blinkrate, triggercb_t callback );
    Atm_comparator & threshold( uint16_t * v, uint16_t size );
    Atm_comparator & average( uint16_t * v, uint16_t size );
    int _avg();
    Atm_comparator & bitmap( int v );
    int sample();
    virtual int read_sample();
    int event( int id ); 
    void action( int id ); 
};

#endif
