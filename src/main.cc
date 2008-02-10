#include <iostream>

#include "main.hh"
#include "party.hh"
#include "world.hh"
#include "journey.hh"

using std::cin;
using std::cout;
using std::endl;

int main (int argc, char **argv)
{
	party* the_party;
	world* the_world;
	the_world  = new world (75); //XXX: Replace with randomness
	the_party = new party;

	if (the_party == NULL || the_world == NULL)
	{
		std::cerr << "Could not allocate resources.\n";
		exit (1);
	}

	the_party->init_party();
	journey::init(the_party, the_world);

	while (the_world->get_next_loc())
	{
		journey::run_instance();
	}

	delete the_world;
	delete the_party;
	return 0;
}

