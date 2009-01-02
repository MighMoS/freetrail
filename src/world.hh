#ifndef WORLD
#define WORLD
#include <vector>

#include <glibmm.h>

#ifdef DEBUG
#include <iostream>
#endif

#include "common.hh"

class location
{
    Glib::ustring name;
    unsigned int next_distance; // Distance to the next stop
    bool is_outpost;
    bool can_hunt;

    public:
    location (const std::string& its_name, const unsigned int distance,
              const bool outpost, const bool hunting);
    Glib::ustring get_name () const {return name;};
    unsigned int get_next_distance () const {return next_distance;};
#ifdef DEBUG
    friend ostream& operator <<(ostream& os, const location& loc);
#endif
};

class Track
{
    std::vector<location> track;
    int track_number; // May or may not need this
    public:
    Track(const int number);
    const location* get_stop(const unsigned int pos) const;
    unsigned int size() const;
    void add_location(const location& loc);
};

class Map
{
    std::vector<Track> all_tracks;
    public:
    void add_track(const Track& track);
    const Track* get_track(const unsigned int pos) const;
    unsigned int get_number_tracks() const;
    unsigned int get_track_size(unsigned int track_no) const;
};

class World
{
    private:
    Map* map;

    public:
    World();

    // The following is a hack, because we can't remove World yet
    const Map* get_map() const;

    location* get_next_loc();
    const location* get_curr_loc(const unsigned int track,
                                 const unsigned int pos) const;

    bool no_more() const;
};

#endif
