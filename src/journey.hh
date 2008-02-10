#ifndef JOURNEY
#define JOURNEY
#include "party.hh"
#include "world.hh"

class journey
{
	private:
	static const unsigned int get_remaining_distance();
	static world* the_world;
	static party* the_party;

	public:
	static void init(party* _party, world* _world);
	static void run_instance();
	static void stop_and_shop();
};

#endif
