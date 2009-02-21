#include <iostream>

#include "journey.hh"
#include "main.hh"
#include "party.hh"
#include "runners.hh"
#include "ui.hh"
#include "world.hh"

int main (int argc, char *argv[])
{
    Party* the_party;
    World* the_world;
    the_world  = new World ();
    the_party = new Party;
    Freetrail::Runner::IMapRunner mRun (the_party, the_world->get_map());
    Freetrail::Runner::Status stat;

    if (the_party == NULL || the_world == NULL)
    {
        std::cerr << "Could not allocate resources.\n";
        return 1;
    }

    user_interface::welcome_new_user();

    the_party->init_party();
    Journey::init(the_party, the_world);

    stat = mRun.run ();
    if (stat.hasWon ())
        user_interface::win(*the_party);

    delete the_world;
    delete the_party;
    return 0;
}

