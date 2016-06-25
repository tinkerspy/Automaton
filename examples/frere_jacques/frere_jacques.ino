#include <Automaton.h>
#include "musical_notes.h"

// Playback 'Frere Jacques' with button trigger and speed control 

Atm_player player;
Atm_button button;
Atm_analog speed;

int pattern[] = { 
  _G4, _N04, 0, _A4, _N04, 0, _B4, _N04, 0, _G4, _N04, 0, // Frere Jacques
  _G4, _N04, 0, _A4, _N04, 0, _B4, _N04, 0, _G4, _N04, 0, 
  _B4, _N04, 0, _C5, _N04, 0, _D5, _N04, _N04, // Dormez vous?
  _B4, _N04, 0, _C5, _N04, 0, _D5, _N04, _N04, 
  _D5, _N08, 0, _E5, _N08, 0, _D5, _N08, 0, _C5, _N08, 0, _B4, _N04, 0, _G4, _N04, 0, // Sonnez les matines 
  _D5, _N08, 0, _E5, _N08, 0, _D5, _N08, 0, _C5, _N08, 0, _B4, _N04, 0, _G4, _N04, 0, 
  _G4, _N04, 0, _D4, _N04, 0, _G4, _N04, _N04, // Ding dang dong
  _G4, _N04, 0, _D4, _N04, 0, _G4, _N04, _N04,
};

void setup() {
  player.begin( 19 ) // A passive buzzer or speaker on pin 19
    .play( pattern, sizeof( pattern ) )
    .repeat( -1 );

  button.begin( 2 ) // A button on pin 2 toggles playback on and off
    .onPress( player, player.EVT_TOGGLE );

  speed.begin( A0 ) // An analog pot on pin A0 controls playback speed
    .range( 50, 300 ) // From 50% to 300% of original speed
    .onChange( []( int idx, int v, int up ) {
      player.speed( v );    
    });
}

void loop() {
  automaton.run();
}

