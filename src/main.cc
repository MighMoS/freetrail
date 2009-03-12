#include <iostream>

#include <glibmm.h>

#include "journey.hh"
#include "main.hh"
#include "parser.hh"
#include "party.hh"
#include "runners.hh"
#include "ui.hh"
#include "world.hh"

int main (int argc, char *argv[])
{
    Party* the_party;
    Map* the_map;

    Glib::set_prgname ("Freetrail");
    try
    {
        the_party = new Party;
        the_map = new Map;
    }
    catch (MapParsingException e)
    {
        std::cerr << "ERROR LOADING THE MAP because:\n\t";
        std::cerr << e.what() << std::endl;
        exit (2);
    }

    Freetrail::Runner::IMapRunner mRun (the_party, the_map);
    Freetrail::Runner::Status stat;

    if (the_party == NULL)
    {
        std::cerr << "Could not allocate resources.\n";
        return 1;
    }

    user_interface::welcome_new_user ();

    the_party->init_party ();
    Journey::init (the_party);

    stat = mRun.run ();
    if (stat.hasWon ())
        user_interface::win (*the_party);
    else
        user_interface::lose ();

    delete the_party;
    return 0;
}

