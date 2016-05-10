#!/bin/sh

clang-format -i Atm_bit.cpp
clang-format -i Atm_pulse.cpp
clang-format -i Atm_condition.cpp

tools/tinyfy.pl Atm_bit.cpp > Att_bit.cpp
tools/tinyfy.pl Atm_bit.hpp > Att_bit.hpp
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

