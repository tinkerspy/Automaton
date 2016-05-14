#!/bin/sh -x

clang-format -i src/Automaton.cpp
clang-format -i src/Automaton.h

clang-format -i src/Machines/Atm_analog_in.cpp
clang-format -i src/Machines/Atm_bit.cpp
clang-format -i src/Machines/Atm_button.cpp
clang-format -i src/Machines/Atm_command.cpp
clang-format -i src/Machines/Atm_comparator.cpp
clang-format -i src/Machines/Atm_condition.cpp
clang-format -i src/Machines/Atm_encoder.cpp
clang-format -i src/Machines/Atm_fade.cpp
clang-format -i src/Machines/Atm_multiplier.cpp
clang-format -i src/Machines/Atm_led.cpp
clang-format -i src/Machines/Atm_digital_in.cpp
clang-format -i src/Machines/Atm_step.cpp
clang-format -i src/Machines/Atm_timer.cpp

extras/tinyfy.pl src/Machines/Atm_analog_in.cpp > src/TinyMachines/Att_analog_in.cpp
extras/tinyfy.pl src/Machines/Atm_analog_in.hpp > src/TinyMachines/Att_analog_in.hpp
extras/tinyfy.pl src/Machines/Atm_button.cpp > src/TinyMachines/Att_button.cpp
extras/tinyfy.pl src/Machines/Atm_button.hpp > src/TinyMachines/Att_button.hpp
extras/tinyfy.pl src/Machines/Atm_command.cpp > src/TinyMachines/Att_command.cpp
extras/tinyfy.pl src/Machines/Atm_command.hpp > src/TinyMachines/Att_command.hpp
extras/tinyfy.pl src/Machines/Atm_comparator.cpp > src/TinyMachines/Att_comparator.cpp
extras/tinyfy.pl src/Machines/Atm_comparator.hpp > src/TinyMachines/Att_comparator.hpp
extras/tinyfy.pl src/Machines/Atm_condition.cpp > src/TinyMachines/Att_condition.cpp
extras/tinyfy.pl src/Machines/Atm_condition.hpp > src/TinyMachines/Att_condition.hpp
extras/tinyfy.pl src/Machines/Atm_encoder.cpp > src/TinyMachines/Att_encoder.cpp
extras/tinyfy.pl src/Machines/Atm_encoder.hpp > src/TinyMachines/Att_encoder.hpp
extras/tinyfy.pl src/Machines/Atm_fade.cpp > src/TinyMachines/Att_fade.cpp
extras/tinyfy.pl src/Machines/Atm_fade.hpp > src/TinyMachines/Att_fade.hpp
extras/tinyfy.pl src/Machines/Atm_multiplier.cpp > src/TinyMachines/Att_multiplier.cpp
extras/tinyfy.pl src/Machines/Atm_multiplier.hpp > src/TinyMachines/Att_multiplier.hpp
extras/tinyfy.pl src/Machines/Atm_led.cpp > src/TinyMachines/Att_led.cpp
extras/tinyfy.pl src/Machines/Atm_led.hpp > src/TinyMachines/Att_led.hpp
extras/tinyfy.pl src/Machines/Atm_digital_in.cpp > src/TinyMachines/Att_digital_in.cpp
extras/tinyfy.pl src/Machines/Atm_digital_in.hpp > src/TinyMachines/Att_digital_in.hpp
extras/tinyfy.pl src/Machines/Atm_step.cpp > src/TinyMachines/Att_step.cpp
extras/tinyfy.pl src/Machines/Atm_step.hpp > src/TinyMachines/Att_step.hpp
extras/tinyfy.pl src/Machines/Atm_timer.cpp > src/TinyMachines/Att_timer.cpp
extras/tinyfy.pl src/Machines/Atm_timer.hpp > src/TinyMachines/Att_timer.hpp

(
chmod -x *.cpp *.hpp *.h *.ino
chmod -x */*.cpp */*.hpp */*.h */*.ino
chmod -x */*/*.cpp */*/*.hpp */*/*.h */*/*.ino
) 2> /dev/null

git add .

