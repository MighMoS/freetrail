#ifndef WORLD
#define WORLD
#include "party.hh"
#include "common.hh"

class world
{
	private:
	int temperature; // In degrees F
	weather the_weather;
	public:
	world(int temp, weather conditions=SUNNY);
	int get_temp() const;
	weather get_conditions() const;
	void set_temp(int);
	void set_conditions(weather);
};

#endif
