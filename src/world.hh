#ifndef WORLD
#define WORLD
#include <vector>
using std::vector;
#ifdef DEBUG
#include <iostream>
using std::ostream;
#endif

#include "common.hh"

class location
{
	public:
	string name;
	int distance;
	bool is_outpost;
	bool can_hunt;
	location(const string& its_name);
#ifdef DEBUG
	friend ostream& operator <<(ostream& os, const location& loc);
#endif
};

class world
{
	private:
	vector<location> map;
	int temperature; // In degrees F
	weather the_weather;

	public:
	world(const int temp, const weather conditions=SUNNY);
	int get_temp() const;
	weather get_conditions() const;
	void set_temp(const int);
	void set_conditions(const weather);
};

#endif
