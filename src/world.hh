#ifndef WORLD
#define WORLD
#include <vector>

#include <glibmm.h>

#ifdef DEBUG
#include <iostream>
#endif

#include "common.hh"

class Map;

/// Container class holding where something is.
class Coordinates
{
    public:
        unsigned int track_number; ///< Which track (0 based)
        unsigned int track_position; ///< Where on the track (0 based)
};

/// Abstract class for "somewhere".
class location
{
    protected:
    Glib::ustring name_;
    /// Is this a settled place?
    /// @deprecated: In the future a new class (Outpost) should be used.
    bool is_outpost;

    /// Can we hunt?
    ///@deprecated: I'm not sure what to do with this, but it seems wrong.
    bool can_hunt;

    friend class Journey;
    public:
    location(const Glib::ustring& name) : name_(name) {};
    /// Returns a friendly name for this place.
    Glib::ustring get_name () const {return name_;};
    //virtual Freetrail::Runner::Status run (Party* party) = 0;
    virtual std::vector<location*>
        getNext(const unsigned int track_no,
                const unsigned int position) const = 0;

#ifdef DEBUG
    friend ostream& operator <<(ostream& os, const location& loc);
#endif
};

// For compat:
typedef location Location;

/// Reserved for future use.
//
/// Will contain a list of items for sale (and price), for one.
class Outpost : public location
{
};

/// A road to travel, the most common type of location.
class Path : public location
{
    unsigned int next_distance; // Distance to the next stop
    location* _next_location; ///<-- What's next? TODO: be smarter.

    public:
    Path (const Glib::ustring& name, const unsigned int distance,
              const bool outpost, const bool hunting):
        location(name), next_distance(distance) {};
    /// How far away is the next location, from start to finish.
    unsigned int get_next_distance () const {return next_distance;};
    /// Should only be called by map parser.
    void set_next_location(location* next_location);
    virtual std::vector<location*>
        getNext(const unsigned int track_no, const unsigned int position) const;
};

/// Container object holding a location, and a "how to get there".
class ForkOption
{
    const Glib::ustring description_;
    const unsigned int destination_;

    public:
    ForkOption (const Glib::ustring& desc, const unsigned int dest) :
        description_(desc), destination_(dest)
    {};
    /// Returns what's special about this path
    const Glib::ustring& get_description () const {return description_;};
    /// Returns an int pointing to the track to jump to
    unsigned int get_destination () const {return destination_;};
};

/// A classic fork in the road.
class Fork : public location
{
    const std::vector<ForkOption> jump_locations_;

    const std::vector<ForkOption>& get_jumps () {return jump_locations_;};
    public:
    Fork (const Glib::ustring& name,
          const std::vector<ForkOption>& jump_locations) :
        location(name), jump_locations_(jump_locations)
    {};
    /// Returns all the places someone can go from here.
    virtual std::vector<location*>
        getNext(const unsigned int track_no, const unsigned int position) const;
};

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
