#!/usr/bin/perl

use warnings;
use strict;

while (<>) {
  s/^#undef\s+TINYMACHINE/#define TINYMACHINE/g;
  s/Atm_/Att_/g;
  print;

}

