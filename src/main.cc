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
    Glib::set_prgname ("Freetrail");

    user_interface::welcome_new_user ();

    const Map the_map = user_interface::getMapChoice ();
    Party the_party = user_interface::createParty ();

    Freetrail::Runner::IMapRunner mRun (the_party, the_map);
    Freetrail::Runner::Status stat;

    stat = mRun.run ();
    if (stat.hasWon ())
        user_interface::win (the_party);
    else
        user_interface::lose ();

    return 0;
}

