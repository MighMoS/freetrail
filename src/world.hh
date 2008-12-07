#ifndef WORLD
#define WORLD
#include <vector>

#ifdef DEBUG
#include <iostream>
#endif

#include "common.hh"

class location
{
	public:
        std::string name;
	unsigned int distance;
	bool is_outpost;
	bool can_hunt;
	location(const std::string& its_name);
#ifdef DEBUG
	friend ostream& operator <<(ostream& os, const location& loc);
#endif
};

class World
{
	private:
    std::vector<location> map;
	int temperature; // In degrees F
	weather the_weather;

	public:
	World(const int temp, const weather conditions=SUNNY);
	int get_temp() const;
	weather get_conditions() const;
	void set_temp(const int);
	void set_conditions(const weather);

	location* get_next_loc();
	location* get_curr_loc();

	void pop_curr_loc();
	bool no_more() const;
};

#endif
