#ifndef WORLD
#define WORLD
#include <tr1/memory>

#include <vector>

#ifdef DEBUG
#include <iostream>
#endif

#include <glibmm.h>

#include "location.hh"

/// Container holding a list of locations. Best thought of as an "area".
class Track
{
    std::vector<std::tr1::shared_ptr<Location> > _track; ///< Holds a list of our Locations
    Glib::ustring _name; ///< Friendly name describing this area
    public:
    /// Default constructor, creates a Track with no Locations and the provided name.
    Track (const Glib::ustring& name);
    const Location* get_stop(const unsigned int pos) const;
    /// Compares a track's name against a string.
    bool operator == (const Glib::ustring& rhs) const;
    /// Returns how many Locations are stored in this Track.
    unsigned int size() const;
    /// Adds the specified Location the end of this track.
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
