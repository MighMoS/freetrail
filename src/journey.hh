#ifndef JOURNEY
#define JOURNEY
#include "party.hh"
#include "world.hh"

class journey
{
	private:
	static const unsigned int get_distance(party* the_party, world* the_world);

	public:
	static void run_instance(party* the_party, world* the_world);
	static void stop_and_shop(party* the_party, world* the_world);
};

#endif
