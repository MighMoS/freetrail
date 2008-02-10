#include <boost/random.hpp>
#include <iostream>
using std::cout;
using std::cin;
using std::endl;

#include "common.hh"
#include "journey.hh"
#include "party.hh"
#include "ui.hh"
#include "world.hh"

/* Initializes our journey class so that we don't have to keep passing
 * in party and world pointers. 
 */
// TODO: Ideally this function would do any other initialization that's needed,
// but we're not there, yet.
world* journey::the_world = NULL;
party* journey::the_party = NULL;
void journey::init(party* _party,  world* _world)
{
	the_party = _party;
	the_world = _world;
}

/* Runs the party through one 'instance
 * Takes a party, and is free to modify it as it sees fit
 *   That's bad, I know
 * Steps: 
 *   Add distance traveled for that [period of time]
 *   Stop at outposts, if necessary (and end)
 *   Otherwise, run random events (sickness, etc.), and present options
 */
void journey::run_instance()
{
	unsigned int speed;
	unsigned int distance_traveled;
	bool reached_landmark;

	const location* current_landmark = the_world->get_curr_loc();
	//const location* next_landmark = the_world->get_next_loc();

	// Don't run past an outpost
	speed = the_party->get_speed();
	if (journey::get_distance() > speed)
	{
		distance_traveled = speed;
		reached_landmark = false;
	}
	else
	{
		distance_traveled = journey::get_distance();
		reached_landmark = true;
	}
	the_party->add_distance(speed);

	cout << "You traveled " << distance_traveled << " miles today.\n";
	user_interface::wait_for_key();

	if (reached_landmark)
	{
		cout << "You've arrived at " << current_landmark->name;
		user_interface::wait_for_key();
		journey::stop_and_shop();
		return;
	}


#if 0
	// This RNG is based on time.  That's bad, I know.
	// This one is static so that we don't call twice in the same second
	static boost::mt19937 rng (static_cast <unsigned> (std::time(0)));
	boost::uniform_int<> weather_ (1,4); // Gets us 1-4
	
	// The weather shouldn't shift too much, so restrict the randomness
	boost::uniform_int<> temp_mod_ (-17,17); 
	boost::variate_generator<boost::mt19937&, boost::uniform_int<> > weather (rng, weather_);
	boost::variate_generator<boost::mt19937&, boost::uniform_int<> > temp_mod (rng, temp_mod_);
#endif

}

/* Gets the distance between the party's at and the next
 * real landmark.
 */
// ToDO: This should be fixed to not really need parameters
const unsigned int journey::get_distance()
{
	return (the_world->get_next_loc()->distance - the_party->get_distance());
}

void journey::stop_and_shop()
{
	the_world->pop_curr_loc();
	the_party->shop();
}
