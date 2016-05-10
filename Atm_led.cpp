#include "Atm_led.hpp"

Atm_led& Atm_led::begin(int attached_pin, bool activeLow) {
  static const state_t state_table[] PROGMEM = {
      /*               ON_ENTER    ON_LOOP    ON_EXIT  EVT_ON_TIMER  EVT_OFF_TIMER  EVT_COUNTER  EVT_ON  EVT_OFF  EVT_BLINK  EVT_TOGGLE  EVT_TOGGLE_BLINK ELSE
         */
      /* IDLE      */ ACT_INIT, ATM_SLEEP, -1,        -1,        -1,    -1,   ON, -1,   START, ON,   START, -1,  // LED off
      /* ON        */ ACT_ON,   ATM_SLEEP, -1,        -1,        -1,    -1,   -1, OFF,  START, OFF,  OFF,   -1,  // LED on
      /* START     */ ACT_ON,   -1,        -1,        BLINK_OFF, -1,    DONE, ON, OFF,  START, OFF,  OFF,   -1,  // Start blinking
      /* BLINK_OFF */ ACT_OFF,  -1,        -1,        -1,        START, DONE, ON, IDLE, START, IDLE, IDLE,  -1,
      /* DONE      */ -1,       -1,        ACT_CHAIN, -1,        IDLE,  -1,   ON, IDLE, START, IDLE, IDLE,  -1,    // Wait after last blink
      /* OFF       */ ACT_OFF,  -1,        -1,        -1,        -1,    -1,   -1, -1,   START, IDLE, IDLE,  IDLE,  // All off -> IDLE
  };
  Machine::begin(state_table, ELSE);
  pin = attached_pin;
  _activeLow = activeLow;
  pinMode(pin, OUTPUT);
  digitalWrite(pin, _activeLow ? HIGH : LOW);
  on_timer.set(500);
  off_timer.set(500);
  repeat_count = ATM_COUNTER_OFF;
  counter.set(repeat_count);
  while (state() != 0) cycle();
  return *this;
}

Atm_led& Atm_led::onFinish(Machine& n, uint8_t event /* = EVT_BLINK */) {
  onFinish(n, n, event);
  return *this;
}

Atm_led& Atm_led::onFinish(Machine& n, Machine& p, uint8_t event /* = EVT_BLINK */) {
  machine_next = &n;
  machine_previous = &p;
  machine_event = event;
  flags &= ~ATM_USR_FLAGS;
  flags |= ATM_USR2_FLAG;
  return *this;
}

Atm_led& Atm_led::onFinish(TinyMachine& n, uint8_t event /* = EVT_BLINK */) {
  onFinish(n, n, event);
  return *this;
}

Atm_led& Atm_led::onFinish(TinyMachine& n, TinyMachine& p, uint8_t event /* = EVT_BLINK */) {
  tmachine_next = &n;
  tmachine_previous = &p;
  machine_event = event;
  flags &= ~ATM_USR_FLAGS;
  flags |= ATM_USR4_FLAG;
  return *this;
}

Atm_led& Atm_led::blink(uint32_t duration, uint32_t pause_duration, uint16_t repeat_count /* = ATM_COUNTER_OFF */) {

  blink(duration);  // Time in which led is fully on
  pause(pause_duration);
  repeat(repeat_count);
  return *this;
}

Atm_led& Atm_led::blink(uint32_t duration) {
  on_timer.set(duration);  // Time in which led is fully on
  return *this;
}

Atm_led& Atm_led::pause(uint32_t duration) {
  off_timer.set(duration);  // Time in which led is fully off
  return *this;
}

Atm_led& Atm_led::fade(int fade) { return *this; }  // Dummy for method compatibility with Atm_fade

Atm_led& Atm_led::repeat(int repeat) {
  repeat_count = repeat >= 0 ? repeat : ATM_COUNTER_OFF;
  counter.set(repeat_count);
  return *this;
}

int Atm_led::event(int id) {
  switch (id) {
    case EVT_ON_TIMER:
      return on_timer.expired(this);
    case EVT_OFF_TIMER:
      return off_timer.expired(this);
    case EVT_COUNTER:
      return counter.expired();
  }
  return 0;
}

void Atm_led::chain_next(void) {

  if ((flags & ATM_USR2_FLAG) > 0) {
    machine_next->trigger(machine_event);
  }
  if ((flags & ATM_USR4_FLAG) > 0) {
    tmachine_next->trigger(machine_event);
  }
}

void Atm_led::chain_previous(void) {

  if ((flags & ATM_USR2_FLAG) > 0) {
    machine_previous->trigger(machine_event);
  }
  if ((flags & ATM_USR4_FLAG) > 0) {
    tmachine_previous->trigger(machine_event);
  }
}

void Atm_led::action(int id) {
  switch (id) {
    case ACT_INIT:
      counter.set(repeat_count);
      return;
    case ACT_ON:
      digitalWrite(pin, _activeLow ? LOW : HIGH);
      return;
    case ACT_OFF:
      counter.decrement();
      digitalWrite(pin, _activeLow ? HIGH : LOW);
      return;
    case ACT_CHAIN:
      if ((flags & (ATM_USR2_FLAG | ATM_USR4_FLAG)) > 0) {
        if ((flags & ATM_USR1_FLAG) > 0) {
          chain_previous();
        } else {
          chain_next();
        }
        flags ^= ATM_USR1_FLAG;
      }
      return;
  }
}

Atm_led& Atm_led::trace(Stream& stream) {

  setTrace(&stream, atm_serial_debug::trace,
           "EVT_ON_TIMER\0EVT_OFF_TIMER\0EVT_COUNTER\0EVT_ON\0EVT_OFF\0EVT_BLINK\0EVT_TOGGLE\0EVT_TOGGLE_BLINK\0ELSE\0"
           "IDLE\0ON\0START\0BLINK_OFF\0DONE\0OFF");
  return *this;
}

// TinyMachine version

Att_led& Att_led::begin(int attached_pin, bool activeLow) {
  static const tiny_state_t state_table[] PROGMEM = {
      /*               ON_ENTER    ON_LOOP    ON_EXIT  EVT_ON_TIMER  EVT_OFF_TIMER  EVT_COUNTER  EVT_ON  EVT_OFF  EVT_BLINK  EVT_TOGGLE  EVT_TOGGLE_BLINK ELSE
         */
      /* IDLE      */ ACT_INIT, ATM_SLEEP, -1,        -1,        -1,    -1,   ON, -1,   START, ON,   START, -1,  // LED off
      /* ON        */ ACT_ON,   ATM_SLEEP, -1,        -1,        -1,    -1,   -1, OFF,  START, OFF,  OFF,   -1,  // LED on
      /* START     */ ACT_ON,   -1,        -1,        BLINK_OFF, -1,    DONE, ON, OFF,  START, OFF,  OFF,   -1,  // Start blinking
      /* BLINK_OFF */ ACT_OFF,  -1,        -1,        -1,        START, DONE, ON, IDLE, START, IDLE, IDLE,  -1,
      /* DONE      */ -1,       -1,        ACT_CHAIN, -1,        IDLE,  -1,   ON, IDLE, START, IDLE, IDLE,  -1,    // Wait after last blink
      /* OFF       */ ACT_OFF,  -1,        -1,        -1,        -1,    -1,   -1, -1,   START, IDLE, IDLE,  IDLE,  // All off -> IDLE
  };
  TinyMachine::begin(state_table, ELSE);
  pin = attached_pin;
  _activeLow = activeLow;
  pinMode(pin, OUTPUT);
  digitalWrite(pin, _activeLow ? HIGH : LOW);
  on_timer.set(500);
  off_timer.set(500);
  repeat_count = ATM_COUNTER_OFF;
  counter.set(repeat_count);
  while (state() != 0) cycle();
  return *this;
}

Att_led& Att_led::onFinish(Machine& n, uint8_t event /* = EVT_BLINK */) {
  onFinish(n, n, event);
  return *this;
}

Att_led& Att_led::onFinish(Machine& n, Machine& p, uint8_t event /* = EVT_BLINK */) {
  machine_next = &n;
  machine_previous = &p;
  machine_event = event;
  flags &= ~ATM_USR_FLAGS;
  flags |= ATM_USR2_FLAG;
  return *this;
}

Att_led& Att_led::onFinish(TinyMachine& n, uint8_t event /* = EVT_BLINK */) {
  onFinish(n, n, event);
  return *this;
}

Att_led& Att_led::onFinish(TinyMachine& n, TinyMachine& p, uint8_t event /* = EVT_BLINK */) {
  tmachine_next = &n;
  tmachine_previous = &p;
  machine_event = event;
  flags &= ~ATM_USR_FLAGS;
  flags |= ATM_USR4_FLAG;
  return *this;
}

Att_led& Att_led::blink(uint32_t duration, uint32_t pause_duration, uint16_t repeat_count /* = ATM_COUNTER_OFF */) {

  blink(duration);  // Time in which led is fully on
  pause(pause_duration);
  repeat(repeat_count);
  return *this;
}

Att_led& Att_led::blink(uint32_t duration) {
  on_timer.set(duration);  // Time in which led is fully on
  return *this;
}

Att_led& Att_led::pause(uint32_t duration) {
  off_timer.set(duration);  // Time in which led is fully off
  return *this;
}

Att_led& Att_led::fade(int fade) { return *this; }  // Dummy for method compatibility with Att_fade

Att_led& Att_led::repeat(int repeat) {
  repeat_count = repeat >= 0 ? repeat : ATM_COUNTER_OFF;
  counter.set(repeat_count);
  return *this;
}

void Att_led::chain_next(void) {

  if ((flags & ATM_USR2_FLAG) > 0) {
    machine_next->trigger(machine_event);
  }
  if ((flags & ATM_USR4_FLAG) > 0) {
    tmachine_next->trigger(machine_event);
  }
}

void Att_led::chain_previous(void) {

  if ((flags & ATM_USR2_FLAG) > 0) {
    machine_previous->trigger(machine_event);
  }
  if ((flags & ATM_USR4_FLAG) > 0) {
    tmachine_previous->trigger(machine_event);
  }
}

int Att_led::event(int id) {
  switch (id) {
    case EVT_ON_TIMER:
      return on_timer.expired(this);
    case EVT_OFF_TIMER:
      return off_timer.expired(this);
    case EVT_COUNTER:
      return counter.expired();
  }
  return 0;
}

void Att_led::action(int id) {
  switch (id) {
    case ACT_INIT:
      counter.set(repeat_count);
      return;
    case ACT_ON:
      digitalWrite(pin, _activeLow ? LOW : HIGH);
      return;
    case ACT_OFF:
      counter.decrement();
      digitalWrite(pin, _activeLow ? HIGH : LOW);
      return;
    case ACT_CHAIN:
      if ((flags & (ATM_USR2_FLAG | ATM_USR4_FLAG)) > 0) {
        if ((flags & ATM_USR1_FLAG) > 0) {
          chain_previous();
        } else {
          chain_next();
        }
        flags ^= ATM_USR1_FLAG;
      }
      return;
  }
}
