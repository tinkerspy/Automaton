#!/bin/sh -x

clang-format -i src/Automaton.cpp src/Automaton.h src/*.cpp src/*.hpp

(
chmod -x *.cpp *.hpp *.h *.ino
chmod -x */*.cpp */*.hpp */*.h */*.ino
chmod -x */*/*.cpp */*/*.hpp */*/*.h */*/*.ino
) 2> /dev/null

git add .

