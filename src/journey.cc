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
World* Journey::the_world = NULL;
Party* Journey::the_party = NULL;
void Journey::init(Party* _party,  World* _world)
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
void Journey::run_instance()
{
	unsigned int speed;
	unsigned int distance_traveled;
	unsigned int food_eaten;
	bool reached_landmark;

	const location* current_landmark = the_world->get_curr_loc();

	// Don't run past an outpost
	speed = the_party->get_speed();
	if (Journey::get_remaining_distance() > speed)
	{
		distance_traveled = speed;
		reached_landmark = false;
	}
	else
	{
		distance_traveled = Journey::get_remaining_distance();
		reached_landmark = true;
	}
	the_party->add_distance(speed);
	food_eaten = the_party->eat_food();

	cout << "You traveled " << distance_traveled << " miles today and have "
		<< the_party->get_food() << " lbs of food remaining.\n";
	user_interface::wait_for_key();

	if (reached_landmark)
	{
		cout << "You've arrived at " << current_landmark->name << endl;
		user_interface::wait_for_key();
		Journey::stop_and_shop();
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
// TODO: This should be fixed to not really need parameters
const unsigned int Journey::get_remaining_distance()
{
    return 50;
	//return (the_world->get_next_loc()->next_distance - the_party->get_distance());
}

void Journey::stop_and_shop()
{
	the_world->pop_curr_loc();
	the_party->shop();
}
