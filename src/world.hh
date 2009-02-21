#ifndef WORLD
#define WORLD
#include <vector>

#ifdef DEBUG
#include <iostream>
#endif

#include <glibmm.h>

#include "location.hh"

class Map;

/// Container holding a list of locations.
class Track
{
    std::vector<const location*> track; ///< Holds a list of our locations
    int track_number; ///< May or may not need this
    public:
    Track(const int number);
    const location* get_stop(const unsigned int pos) const;
    unsigned int size() const;
    void add_location(location* loc);
};

/// Container holding tracks, which in turn hold locations.
class Map
{
    std::vector<Track> all_tracks;
    public:
    void add_track(const Track& track);
    /// Gets the first track of the map. This must exist and can not fail.
    const Track* getStartTrack() const;
    const Track* get_track(const unsigned int pos) const;
    /// Returns the number of tracks held.
    unsigned int size() const;
};

/// Generic functions for interacting with a map* when one isn't available.
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
