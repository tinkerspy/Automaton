#include <Automaton.h>
#include <Atm_scope.h>

// This sketch runs a 'scope' machine on pin 3
// causing pin 3 to be toggled at the maximum machine speed

Atm_scope scope;
Factory factory;

void setup()
{
  scope.begin( 3 );
  factory.add( scope );
}

void loop()
{
  factory.cycle();
}

