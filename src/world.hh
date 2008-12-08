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
	unsigned int next_distance; // Distance to the next stop
	bool is_outpost;
	bool can_hunt;
	location(const std::string& its_name, const unsigned int distance,
             const bool outpost, const bool hunting);
#ifdef DEBUG
	friend ostream& operator <<(ostream& os, const location& loc);
#endif
};

class Track
{
    std::vector<location> track;
    int track_number; // May or may not need this
    public:
    void add_location(const location& loc);
    Track(const int number);
};

class Map
{
    std::vector<Track> all_tracks;
    public:
    void add_track(const Track& track);
};

class World
{
	private:
    Map* map;
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
