#include <iostream>
using std::cout;
using std::endl;
#include <fstream>
#include <string>
using std::string;
#include <vector>
using std::vector;

#include "common.hh"
#include "world.hh"

static void parse_locations(vector<location>& map, char* filename = "map.ini")
{
	string location_name, buffer;
	std::ifstream mapfile;
	mapfile.open (filename);
	if (!mapfile)
	{
		std::cerr << "Error opening " << filename <<". Exiting.\n";
		exit (1);
	}
	while (getline (mapfile, buffer))
	{
		if (buffer.length() == 0) // Blank line
			continue;
		string::iterator last_char = buffer.end();
		last_char--;
#ifdef DEBUG
		cout << "Got buffer of size " << buffer.length() <<endl;
		cout << "\tIt read "<< buffer << endl;
		cout << "\tBuffer[0] is " << buffer[0] 
			<< "\tand buffer[last] is " << *last_char << endl;
#endif

		// We got a name
		if (buffer[0] == '[' && *last_char == ']')
		{
			string temp_name = buffer;
			temp_name.erase(temp_name.begin());
			temp_name.erase(temp_name.end()-1);
#ifdef DEBUG
			cout << "Pushing back new location with name: " 
				<< temp_name << endl;
#endif
			location temp_location (temp_name);
			map.push_back(temp_location);
			continue;
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
