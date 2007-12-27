#include <boost/random.hpp>
#include <iostream>
using std::cout;
using std::cin;
using std::endl;

#include "common.hh"
#include "world.hh"

/* Runs the party through one 'instance
 * Takes a party, and is free to modify it as it sees fit
 */
void run_instance(party* the_party, world* the_world)
{
	// This RNG is based on time.  That's bad, I know.
	// This one is static so that we don't call twice in the same second
	boost::mt19937 rng (static_cast <unsigned> (std::time(0)));
	boost::uniform_int<> weather_ (1,4); // Gets us 1-4
	
	// The weather shouldn't shift too much, so restrict the randomness
	boost::uniform_int<> temp_mod_ (-17,17); 
	boost::variate_generator<boost::mt19937&, boost::uniform_int<> > weather (rng, weather_);
	boost::variate_generator<boost::mt19937&, boost::uniform_int<> > temp_mod (rng, temp_mod_);
}
