#include <cassert>
//#include <boost/random.hpp>
#include <iostream>
using std::cout;
using std::cin;
using std::endl;

#include "common.hh"
#include "journey.hh"
#include "party.hh"
#include "ui.hh"
#include "world.hh"

World* Journey::the_world = NULL;
Party* Journey::the_party = NULL;

/// Initializes our journey class so that we don't have to keep passing
/// in party and world pointers.
/**
 * @todo Ideally this function would do any other initialization that's needed,
 *  but we're not there, yet.
 */
void Journey::init(Party* _party,  World* _world)
{
    the_party = _party;
    the_world = _world;
}

