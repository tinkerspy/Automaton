#!/usr/bin/perl

use warnings;
use strict;

#define FACTORY factory
#define STATE_TYPE state_t
#define MACHINE Machine

while (<>) {
  s/Atm_/Att_/g;
  s/^#define\s+FACTORY\s+factory\b/#define FACTORY 0/g;
  s/^#define\s+STATE_TYPE\s+state_t\b/#define STATE_TYPE tiny_state_t/g;
  s/^#define\s+MACHINE\s+Machine\b/#define MACHINE TinyMachine/g;
  s/^#undef\s+TINYMACHINE/#define TINYMACHINE/g;
  print;
}

