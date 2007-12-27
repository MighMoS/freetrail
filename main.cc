#include <iostream>

#include "main.hh"
#include "party.hh"
#include "world.hh"

using std::cin;
using std::cout;
using std::endl;

int main (int argc, char **argv)
{
	party* the_party;
	world* the_world = new world (75); //XXX: Replace with randomness
	the_party = init_recruit ();

	return 0;
}

party* init_recruit ()
{
	party* some_party = new party;
	some_party->init_party();
	return some_party;
}
