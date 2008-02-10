#include <boost/random.hpp>
#include <iostream>
using std::cout;
using std::cin;
using std::endl;

#include "common.hh"
#include "party.hh"
#include "world.hh"

/* Runs the party through one 'instance
 * Takes a party, and is free to modify it as it sees fit
 *   That's bad, I know
 * Steps: 
 *   Add distance traveled for that [period of time]
 *   Stop at outposts, if necessary (and end)
 *   Otherwise, run random events (sickness, etc.), and present options
 */
void run_instance(party* the_party, world* the_world)
{
	short speed;
	const location* current_landmark = the_world->get_curr_loc();
	const location* next_landmark = the_world->get_next_loc();

	// Don't run past an outpost
	speed = the_party->get_speed();
	if (the_party->get_distance() + speed < next_landmark->distance)
		the_party->add_distance (speed);
	else
		speed = next_landmark->distance - the_party->get_distance();
	cout << "You traveled " << speed << " miles today.\n";

	if (the_party->get_distance() == current_landmark->distance)
	{
		cout << "You've arrived at " << current_landmark->name;
		the_party->shop();
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
