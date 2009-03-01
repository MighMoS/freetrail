#ifndef LOCATION_H
#define LOCATION_H

#include <glibmm.h>

#include "party.hh"

// Found in runners.hh
namespace Freetrail{namespace Runner{
class Status;
}}

/// Abstract class for "somewhere".
class location
{
    protected:
    Glib::ustring _name;
    /// Is this a settled place?
    /// @deprecated: In the future a new class (Outpost) should be used.
    bool is_outpost;

    /// Can we hunt?
    ///@deprecated: I'm not sure what to do with this, but it seems wrong.
    bool can_hunt;

    public:
    location(const Glib::ustring& name) : _name(name) {};
    /// Returns a friendly name for this place.
    Glib::ustring get_name () const {return _name;};
    virtual bool operator == (const Glib::ustring& rhs) const;
    virtual Freetrail::Runner::Status run (Party* party) = 0;

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
    public:
    Outpost (const Glib::ustring& name);
    Freetrail::Runner::Status run (Party* party);
};

/// A road to travel, the most common type of location.
class Path : public location
{
    unsigned int _next_distance; // Distance to the next stop

    public:
    Path (const Glib::ustring& name, const unsigned int distance) :
        location(name), _next_distance(distance) {};
    /// How far away is the next location, from start to finish.
    unsigned int get_next_distance () const {return _next_distance;};
    /// Should only be called by map parser.
    void set_next_location(location* next_location);
    Freetrail::Runner::Status run (Party* party);
};

/// Container object holding a location, and a "how to get there".
class ForkOption
{
    const Glib::ustring _description; ///<A friendly description of what's here.
    const Glib::ustring _destination; ///< Name of the destination track.

    public:
    ForkOption (const Glib::ustring& desc, const Glib::ustring& dest) :
        _description(desc), _destination(dest)
    {};
    /// Returns what's special about this path
    const Glib::ustring& get_description () const {return _description;};
    /// Returns an int pointing to the track to jump to
    const Glib::ustring& get_destination () const {return _destination;};
};

/// A classic fork in the road.
class Fork : public location
{
    const std::vector<ForkOption*> _jump_locations; ///<Possible places we could go.

    public:
    Fork (const Glib::ustring& name,
          const std::vector<ForkOption*>& jump_locations) :
        location(name), _jump_locations(jump_locations)
    {};
    ~Fork();

    Freetrail::Runner::Status run (Party* party);
    const std::vector<ForkOption*>& get_jumps () const
        {return _jump_locations;};
};

#endif // LOCATION_H
