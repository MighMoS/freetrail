#ifndef WORLD
#define WORLD
#include <vector>

#include <glibmm.h>

#ifdef DEBUG
#include <iostream>
#endif

#include "common.hh"

class ForkOption
{
    const Glib::ustring description_;
    const unsigned int destination_;

    public:
    ForkOption (const Glib::ustring& desc, const unsigned int dest) :
        description_(desc), destination_(dest)
    {};
    const Glib::ustring& get_description () const {return description_;};
    unsigned int get_destination () const {return destination_;};
};

class Fork
{
    const Glib::ustring circumstances_;
    const std::vector<ForkOption> jump_locations_;

    public:
    Fork (const Glib::ustring& circumstances,
          const std::vector<ForkOption>& jump_locations) :
        circumstances_(circumstances), jump_locations_(jump_locations)
    {};
    const Glib::ustring& get_circumstances () const {return circumstances_;};
    const std::vector<ForkOption>& get_jumps () {return jump_locations_;};
};

class location
{
    Glib::ustring name;
    unsigned int next_distance; // Distance to the next stop
    bool is_outpost;
    bool can_hunt;
    const std::vector<Fork>* fork_;

    public:
    location (const std::string& its_name, const unsigned int distance,
              const bool outpost, const bool hunting,
              const std::vector<Fork>* jumps = NULL);
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
    ~World();

    // The following is a hack, because we can't remove World yet
    const Map* get_map() const;

    location* get_next_loc();
    const location* get_curr_loc(const unsigned int track,
                                 const unsigned int pos) const;
};

#endif
