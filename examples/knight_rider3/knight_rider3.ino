#include <Automaton.h>

Atm_player player; // A player machine

const int ledPinMin = 4; // Use pins 4..9
const int ledPinMax = 9; 

int pattern[] = {  // Bitmapped pattern
  B00100000, 100, 0, 
  B00010000, 100, 0, 
  B00001000, 100, 0, 
  B00000100, 100, 0, 
  B00000010, 100, 0, 
  B00000001, 100, 0, 
  B00000010, 100, 0, 
  B00000100, 100, 0, 
  B00001000, 100, 0, 
  B00010000, 100, 0, 
};

void setup() {
  player.begin() 
    .play( pattern, sizeof( pattern ) ) //  Set up the pattern
    .onNote( true, []( int idx, int v, int up ) { // Called on every note
      for ( int i = ledPinMin; i <= ledPinMax; i++ ) {
        pinMode( i, OUTPUT ); // LED on/off according to bit  
        digitalWrite( i, v & ( 1 << ( i - ledPinMin ) ) ? HIGH : LOW ); 
      }    
    })
    .repeat( -1 ) // Repeat forever
    .start(); // Kickoff!
}

void loop() {
  automaton.run();
}

