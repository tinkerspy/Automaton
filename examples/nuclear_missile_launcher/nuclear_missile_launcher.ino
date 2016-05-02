#include <Automaton.h>
#include <Atm_button.h>
#include <Atm_led.h>
#include <Atm_timer.h>
#include <Atm_gate.h>
#include <Atm_step.h>

// Safe controller for a Nuclear Missile Launcher

Atm_led countdown, ignition;
Atm_button button1, button2;
Atm_step burst1, burst2;
Atm_timer reset_timer;
Atm_gate gate;
Factory factory;

const int pinButton1 = 2;
const int pinButton2 = 3;
const int pinCountdownLed = 4;
const int pinIgnitionLed = 5;
const int buttonIntervalMax = 2000;
const int countdownCount = 10;
const int countdownFlashOn = 100;
const int countdownFlashOff = 900;

void setup() {
  
  // Create two button machines, linked to the burst machines
  button1.begin( pinButton1 ).onPress( burst1, burst1.EVT_STEP );
  button2.begin( pinButton2 ).onPress( burst2, burst2.EVT_STEP );

  // Create two step machines and put them in burst mode
  // On receipt of a EVT_STEP trigger: fires all steps at once
  burst1.begin()
    .onStep( 0,        gate,        gate.EVT_G0  )
    .onStep( 1, reset_timer, reset_timer.EVT_ON  )
    .onStep( 2,   countdown,   countdown.EVT_OFF )
    .trigger( burst1.EVT_BURST );
  burst2.begin()
    .onStep( 0,        gate,        gate.EVT_G1  )
    .onStep( 1, reset_timer, reset_timer.EVT_ON  )
    .onStep( 2,   countdown,   countdown.EVT_OFF )
    .trigger( burst2.EVT_BURST );

  // Create the gate machine with two inputs 
  // When both inputs are triggered, the gate opens and starts the countdown
  gate.begin( 2 ).onOpen( countdown, countdown.EVT_BLINK );

  // Create the reset timer to clear the gate some time after a button press
  reset_timer.begin( buttonIntervalMax ).onTimer( gate, gate.EVT_CLEAR );

  // Create the countdown LED (10 times 1 flash per second), chained to the ignition 
  countdown.begin( pinCountdownLed )
    .blink( countdownFlashOn, countdownFlashOff, countdownCount )
    .onFinish( ignition, ignition.EVT_ON ); 

  // Create the ignition LED (connected in hardware to the missile launch trigger)
  ignition.begin( pinIgnitionLed );

  // Add all machines to the  factory
  factory
    .add( button1 ).add(button2 )
    .add( burst1 ).add( burst2 )
    .add( gate ).add( reset_timer )
    .add( countdown ).add( ignition );
}

void loop() {
  // Run the factory
  factory.cycle();
}

