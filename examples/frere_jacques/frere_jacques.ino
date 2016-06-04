#include <Automaton.h>
#include "musical_notes.h"

// Playback 'Frere Jacques' with button trigger and speed control 

Atm_player player;
Atm_button button;
Atm_analog speed;

int pattern[] = { 
  G4, N04, 0,  A4, N04, 0,  B4, N04, 0,  G4, N04, 0, // Frere Jacques
  G4, N04, 0,  A4, N04, 0,  B4, N04, 0,  G4, N04, 0, 
  B4, N04, 0,  C5, N04, 0,  D5, N04, N04, // Dormez vous?
  B4, N04, 0,  C5, N04, 0,  D5, N04, N04, 
  D5, N08, 0, E5, N08, 0, D5, N08, 0, C5, N08, 0,  B4, N04, 0, G4, N04, 0, // Sonnez les matines 
  D5, N08, 0, E5, N08, 0, D5, N08, 0, C5, N08, 0,  B4, N04, 0, G4, N04, 0, 
  G4, N04, 0, D4, N04, 0, G4, N04, N04, // Ding dang dong
  G4, N04, 0, D4, N04, 0, G4, N04, N04,
};

void setup() {
  player.begin( 19 ) // A passive buzzer or speaker on pin 19
    .play( pattern, sizeof( pattern ) )
    .repeat( -1 );

  button.begin( 2 ) // A button on pin 2 toggle playback on and off
    .onPress( player, player.EVT_TOGGLE );

  speed.begin( A0 ) // An analog pot on pin A0 controls playback speed
    .range( 50, 300 )
    .onChange( []( int idx, int v, int up ) {
      player.speed( v );    
    });
}

void loop() {
  automaton.run();
}

