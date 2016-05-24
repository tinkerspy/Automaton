#include <Automaton.h>

// Safe controller for a Nuclear Missile Launcher

Atm_led countdown, ignition;
Atm_button button1, button2;
Atm_bit bit1, bit2;
Atm_timer timer1, timer2;
Atm_controller ctrl;
Appliance app;

const int pinButton1 = 2;
const int pinButton2 = 3;
const int pinCountdownLed = 8;
const int pinIgnitionLed = 9;
const int buttonIntervalMax = 2000;
const int countdownCount = 10;
const int countdownFlashOn = 100;
const int countdownFlashOff = 900;

void setup() {

  // Self resetting button 1
  app.component( 
    button1.begin( pinButton1 )
      .onPress( bit1, bit1.EVT_ON )
  );
  app.component(
    timer1.begin( buttonIntervalMax )
      .onTimer( bit1, bit1.EVT_OFF )
  );
  app.component(
    bit1.begin( false ).led( 4 )
      .onChange( true, timer1, timer1.EVT_START )
  );
  
  // Self resetting button 2
  app.component( 
    button2.begin( pinButton2 )
      .onPress( bit2, bit2.EVT_ON )
  );
  app.component(
    timer2.begin( buttonIntervalMax )
      .onTimer( bit2, bit2.EVT_OFF )
  );
  app.component(
    bit2.begin( false ).led( 5 )
      .onChange( true, timer2, timer2.EVT_START )
  );

  // Controller
  app.component(
    ctrl.begin( false )
      .IF( bit1 ).AND( bit2 )
      .onChange( true, countdown, countdown.EVT_BLINK )
  );

  // Countdown led
  app.component(
    countdown.begin( pinCountdownLed )
      .blink( countdownFlashOn, countdownFlashOff, countdownCount )
      .onFinish( ignition, ignition.EVT_BLINK )
  );

  // Ignition
  app.component(
    ignition.begin( pinIgnitionLed )
      .repeat( 1 )
  );

}

void loop() {
  // Run the app
  app.run();
}
