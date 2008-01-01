#ifndef WORLD
#define WORLD
#include <vector>
using std::vector;

#include "party.hh"
#include "common.hh"

class location
{
	public:
	string name;
	int distance;
	bool can_buy;
	bool can_sell;
	bool can_recruit;
	bool can_hunt;
	location(const string& its_name);
};

class world
{
	private:
	vector<location> map;
	int temperature; // In degrees F
	weather the_weather;
	public:
	world(int temp, weather conditions=SUNNY);
	~world();
	int get_temp() const;
	weather get_conditions() const;
	void set_temp(int);
	void set_conditions(weather);
};

#endif
