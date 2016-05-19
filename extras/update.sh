#!/bin/sh -x

clang-format -i src/Automaton.cpp src/Automaton.h src/*.cpp src/*.hpp

extras/tinyfy.pl src/Atm_analog.cpp > src/Att_analog.cpp
extras/tinyfy.pl src/Atm_analog.hpp > src/Att_analog.hpp
extras/tinyfy.pl src/Atm_bit.cpp > src/Att_bit.cpp
extras/tinyfy.pl src/Atm_bit.hpp > src/Att_bit.hpp
extras/tinyfy.pl src/Atm_button.cpp > src/Att_button.cpp
extras/tinyfy.pl src/Atm_button.hpp > src/Att_button.hpp
extras/tinyfy.pl src/Atm_command.cpp > src/Att_command.cpp
extras/tinyfy.pl src/Atm_command.hpp > src/Att_command.hpp
extras/tinyfy.pl src/Atm_comparator.cpp > src/Att_comparator.cpp
extras/tinyfy.pl src/Atm_comparator.hpp > src/Att_comparator.hpp
extras/tinyfy.pl src/Atm_condition.cpp > src/Att_condition.cpp
extras/tinyfy.pl src/Atm_condition.hpp > src/Att_condition.hpp
extras/tinyfy.pl src/Atm_encoder.cpp > src/Att_encoder.cpp
extras/tinyfy.pl src/Atm_encoder.hpp > src/Att_encoder.hpp
extras/tinyfy.pl src/Atm_fade.cpp > src/Att_fade.cpp
extras/tinyfy.pl src/Atm_fade.hpp > src/Att_fade.hpp
extras/tinyfy.pl src/Atm_fan.cpp > src/Att_fan.cpp
extras/tinyfy.pl src/Atm_fan.hpp > src/Att_fan.hpp
extras/tinyfy.pl src/Atm_led.cpp > src/Att_led.cpp
extras/tinyfy.pl src/Atm_led.hpp > src/Att_led.hpp
extras/tinyfy.pl src/Atm_digital.cpp > src/Att_digital.cpp
extras/tinyfy.pl src/Atm_digital.hpp > src/Att_digital.hpp
extras/tinyfy.pl src/Atm_step.cpp > src/Att_step.cpp
extras/tinyfy.pl src/Atm_step.hpp > src/Att_step.hpp
extras/tinyfy.pl src/Atm_timer.cpp > src/Att_timer.cpp
extras/tinyfy.pl src/Atm_timer.hpp > src/Att_timer.hpp

(
chmod -x *.cpp *.hpp *.h *.ino
chmod -x */*.cpp */*.hpp */*.h */*.ino
chmod -x */*/*.cpp */*/*.hpp */*/*.h */*/*.ino
) 2> /dev/null

git add .

