#ifndef WORLD
#define WORLD
#include <vector>

#ifdef DEBUG
#include <iostream>
#endif

#include <glibmm.h>

#include "location.hh"

/// Container holding a list of locations. Best thought of as an "area".
class Track
{
    std::vector<Location*> _track; ///< Holds a list of our locations
    Glib::ustring _name; ///< Friendly name describing this area
    public:
    Track (const Glib::ustring& name);
    ~Track ();
    const Location* get_stop(const unsigned int pos) const;
    /// Compares a track's name against a string.
    bool operator == (const Glib::ustring& rhs) const;
    unsigned int size() const;
    void add_location(Location* loc);
};

/// Container holding tracks, which in turn hold locations.
class Map
{
    std::vector<Track> all_tracks;
    const Track* _firstTrack;
    public:
    Map (const char filename[] = "map.xml");
    void add_track(const Track& track);
    /// Gets the first track of the map. This must exist and can not fail.
    const Track* getStartTrack() const;
    const Track* get_track(const unsigned int pos) const;
    /// Returns the number of tracks held.
    unsigned int size() const;
};

#endif
