#include <iostream>
#include <fstream>
#include <string>
using std::string;
#include <vector>
using std::vector;

#include "common.hh"
#include "world.hh"

static void parse_locations(vector<location>* map, char* filename = "map.ini")
{
	string location_name, buffer;
	location* temp_loc;
	int loc_distance;
	bool can_buy, can_sell, can_hunt;
	std::ifstream mapfile;
	mapfile.open (filename);
	if (!mapfile)
	{
		std::cerr << "Error opening " << filename <<". Exiting.\n";
		exit (1);
	}
	while (getline (mapfile, buffer))
	{
		if (buffer[0] == '[' && buffer[buffer.length() - 1 ] == ']')
		{
			string uncut_name = buffer;
			uncut_name.erase(0, 1); // Trim off [
			uncut_name.erase(uncut_name.length() - 1, 1); // Trim off ]
			temp_loc = new location (uncut_name);
		}
	}
}

world::world(int temp, weather conditions) : temperature(temp), 
	the_weather(conditions)
{
	parse_locations(map);
}

world::~world()
{
	if (map)
		delete [] map;
}

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

location::location(const string& its_name) : name(its_name) {};
