#include <iostream>

#include "journey.hh"
#include "main.hh"
#include "party.hh"
#include "ui.hh"
#include "world.hh"

using std::cin;
using std::cout;
using std::endl;

int main (/* int argc, char *argv[]*/)
{
    Party* the_party;
    World* the_world;
    the_world  = new World ();
    the_party = new Party;

    if (the_party == NULL || the_world == NULL)
    {
        std::cerr << "Could not allocate resources.\n";
        return 1;
    }

    user_interface::welcome_new_user();

    the_party->init_party();
    Journey::init(the_party, the_world);

    //XXX This is wrong
    while (!(the_world->no_more()))
    {
        Journey::run_instance();
    }

    delete the_world;
    delete the_party;
    return 0;
}

