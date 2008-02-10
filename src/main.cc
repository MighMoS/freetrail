#include <iostream>

#include "journey.hh"
#include "main.hh"
#include "party.hh"
#include "ui.hh"
#include "world.hh"

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

	user_interface::clear_screen();
	cout << "Welcome to FreeTrail. Adventure and fortune awaits you if you can survive.\n"
		<< "You set out to gather your party and resources...\n";
	user_interface::wait_for_key();

	the_party->init_party();
	journey::init(the_party, the_world);

	//XXX This is wrong
	while (the_world->get_next_loc())
	{
		journey::run_instance();
	}

	delete the_world;
	delete the_party;
	return 0;
}

