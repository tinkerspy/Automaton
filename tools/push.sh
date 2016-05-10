#!/bin/sh -x

clang-format -i Automaton.cpp
clang-format -i Automaton.h

clang-format -i Atm_bit.cpp
clang-format -i Atm_button.cpp
clang-format -i Atm_command.cpp
clang-format -i Atm_multiplier.cpp
clang-format -i Atm_led.cpp
clang-format -i Atm_pulse.cpp
clang-format -i Atm_condition.cpp

tools/tinyfy.pl Atm_bit.cpp > Att_bit.cpp
tools/tinyfy.pl Atm_bit.hpp > Att_bit.hpp
tools/tinyfy.pl Atm_button.cpp > Att_button.cpp
tools/tinyfy.pl Atm_button.hpp > Att_button.hpp
tools/tinyfy.pl Atm_command.cpp > Att_command.cpp
tools/tinyfy.pl Atm_command.hpp > Att_command.hpp
tools/tinyfy.pl Atm_multiplier.cpp > Att_multiplier.cpp
tools/tinyfy.pl Atm_multiplier.hpp > Att_multiplier.hpp
tools/tinyfy.pl Atm_led.cpp > Att_led.cpp
tools/tinyfy.pl Atm_led.hpp > Att_led.hpp
tools/tinyfy.pl Atm_pulse.cpp > Att_pulse.cpp
tools/tinyfy.pl Atm_pulse.hpp > Att_pulse.hpp
tools/tinyfy.pl Atm_condition.cpp > Att_condition.cpp
tools/tinyfy.pl Atm_condition.hpp > Att_condition.hpp

(
chmod -x *.cpp *.hpp *.h *.ino
chmod -x */*.cpp */*.hpp */*.h */*.ino
chmod -x */*/*.cpp */*/*.hpp */*/*.h */*/*.ino
) 2> /dev/null

git add .

