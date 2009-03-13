#include <exception>
#include <iostream>

#include <glibmm.h>

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
        the_map = new Map;
        user_interface::welcome_new_user ();
        the_party = user_interface::init_party ();
    }
    catch (const MapParsingException& e)
    {
        std::cerr << "ERROR LOADING THE MAP because:\n\t";
        std::cerr << e.what() << std::endl;
        return 2;
    }
    catch (const std::bad_alloc& e)
    {
        std::cerr << "\n\n\tI ran out of memory. Sorry.\n";
        return 1;
    }

    Freetrail::Runner::IMapRunner mRun (the_party, the_map);
    Freetrail::Runner::Status stat;

    stat = mRun.run ();
    if (stat.hasWon ())
        user_interface::win (*the_party);
    else
        user_interface::lose ();

    delete the_map;
    delete the_party;
    return 0;
}

