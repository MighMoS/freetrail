#include <iostream>
using std::cout;
using std::endl;
#include <fstream>
#include <string>
using std::string;
#include <sstream>
#include <vector>
using std::vector;

#include "common.hh"
#include "world.hh"

/* Takes a field to search, and a filename.  
 * Makes sure that nothing funnny is going on.
 */
static inline bool check_field(const char field[], const string& buffer)
{
	string compare;
	if (buffer.find(field) == string::npos)
		return false;

	for (int i = 0; i < sizeof(field) - 1; i++)
	{
		if (buffer[i] != field[i])
			return false;
	}
	return true;
}

/* Utility function for parsing our map file.
 * Reads in lines one at a time and then performes the following actions
 * based on the cotents of getline():
 *   If the string is blank, ignore it, get another one
 *   If the string begins with [ or ] it processed as a new entry
 *   If the string contains Buy, Hunt, or Distance, it is added to the 
 *      appropriate field of the last entry
 *   Else the string is ignored, and a warning is printed out.
 */
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

		// Do we have a name?
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

		if (check_field ("Buy", buffer))
		{
			string value;
			string::iterator chop_range;

			value = buffer.substr(buffer.find(" = "));
			chop_range = value.begin();
			value.erase(chop_range, chop_range + 3);
			map.back().can_buy = (value == "Yes");
			continue;
		}

		if (check_field ("Hunt", buffer))
		{
			string value;
			string::iterator chop_range;

			value = buffer.substr(buffer.find(" = "));
			chop_range = value.begin();
			value.erase(chop_range, chop_range + 3);
			map.back().can_hunt = (value == "Yes");
			continue;
		}

		if (check_field ("Distance", buffer))
		{
			string value;
			string::iterator chop_range;

			value = buffer.substr(buffer.find(" = "));
			chop_range = value.begin();
			value.erase(chop_range, chop_range + 3);

			std::stringstream valuestream;
			valuestream << value;
			valuestream >> map.back().distance;
			continue;
		}
	}
#ifdef DEBUG
	for (int i = 0; i < map.size(); i++)
		cout << map[i];
#endif
}

world::world(int temp, weather conditions) : temperature(temp), 
	the_weather(conditions)
{
	parse_locations(map);
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

std::ostream& operator << (std::ostream& os, const location& loc)
{
	os << "\n  Name: " << loc.name;
	os << "\n\tCan Buy: " << loc.can_buy ? "Yes" : "No";
	os << "\n\tCan Hunt: " << loc.can_hunt ? "Yes" : "No";
	return os << "\n\tDistance: " << loc.distance << endl;
}
