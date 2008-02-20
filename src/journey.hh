#ifndef JOURNEY
#define JOURNEY
#include "party.hh"
#include "world.hh"

class Journey
{
	private:
	static const unsigned int get_remaining_distance();
	static World* the_world;
	static Party* the_party;

	public:
	static void init(Party* _party, World* _world);
	static void run_instance();
	static void stop_and_shop();
};

#endif
