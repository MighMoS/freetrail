#include "common.hh"
#include "world.hh"

world::world(int temp, weather conditions) : temperature(temp), 
	the_weather(conditions){};

int world::get_temp () const
{
	return temperature;
}

weather world::get_conditions () const
{
	return the_weather;
}

void world::set_temp (int k)
{
	temperature = k;
}

void world::set_conditions (weather k)
{
	the_weather = k;
}
