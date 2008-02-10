#include <boost/random.hpp>
#include <iostream>
using std::cout;
using std::cin;
using std::endl;

#include "common.hh"
#include "party.hh"
#include "world.hh"
#include "journey.hh"

/* Runs the party through one 'instance
 * Takes a party, and is free to modify it as it sees fit
 *   That's bad, I know
 * Steps: 
 *   Add distance traveled for that [period of time]
 *   Stop at outposts, if necessary (and end)
 *   Otherwise, run random events (sickness, etc.), and present options
 */
void journey::run_instance(party* the_party, world* the_world)
{
	unsigned int speed;
	unsigned int distance_traveled;
	bool reached_landmark;

	const location* current_landmark = the_world->get_curr_loc();
	//const location* next_landmark = the_world->get_next_loc();

	// Don't run past an outpost
	speed = the_party->get_speed();
	if (journey::get_distance(the_party, the_world) > speed)
	{
		distance_traveled = speed;
		reached_landmark = false;
	}
	else
	{
		distance_traveled = journey::get_distance(the_party, the_world);
		reached_landmark = true;
	}
	the_party->add_distance(speed);

	cout << "You traveled " << distance_traveled << " miles today.\n";

	if (reached_landmark)
	{
		cout << "You've arrived at " << current_landmark->name;
		journey::stop_and_shop(the_party, the_world);
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
const unsigned int journey::get_distance(party* the_party, world* the_world)
{
	return (the_world->get_next_loc()->distance - the_party->get_distance());
}

void journey::stop_and_shop(party* the_party, world* the_world)
{
	the_world->pop_curr_loc();
	the_party->shop();
}
