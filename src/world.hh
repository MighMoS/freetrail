#ifndef WORLD
#define WORLD
#include <tr1/memory>

#include <set>
#include <vector>

#include <glibmm.h>

#include "location.hh"

typedef std::vector<std::tr1::shared_ptr<Location> > LocationContainer;
/// Container holding a list of locations. Best thought of as an "area".
class Track
{
    LocationContainer _track; ///< Holds a list of our Locations
    Glib::ustring _name; ///< Friendly name describing this area
    public:
    /// Default constructor, creates a Track with no Locations and the provided name.
    Track (const Glib::ustring& name);
    const Location* get_stop(const unsigned int pos) const;
    /// Compares a track's name against a string.
    bool operator == (const Glib::ustring& rhs) const;
    /// Needed for std::set
    bool operator < (const Track& rhs) const {return _name < rhs._name;};
    /// Adds the specified Location the end of this track.
    void add_location(Location* loc);
    /// Returns the name of this track
    const Glib::ustring& get_name () const {return _name;};
    /// Returns how many Locations are stored in this Track.
    unsigned int size() const;
};

typedef std::set<Track> TrackContainer;
/// Container holding tracks, which in turn hold locations.
class Map
{
    TrackContainer _all_tracks; ///< Container holding everything.
    Glib::ustring _first_track; ///< Name of our starting location.
    public:
    Map (const char filename[] = "map.xml");
    /// Adds the specified Track to the map.
    void add_track(const Track& track);
    /// Returns a pointer to the specified Track.
    const Track* find (const Glib::ustring& track_name) const;
    /// Gets the first Track of the map. This must exist and can not fail.
    const Track* getStartTrack() const;
    /// Returns the number of tracks held.
    unsigned int size() const;
};

#endif
