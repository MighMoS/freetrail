#ifndef LOCATION_H
#define LOCATION_H

#ifndef NDEBUG
#include <iostream>
#endif

#include <glibmm.h>

#include "party.hh"

// Found in runners.hh
namespace Freetrail
{
    namespace Runner
    {
        class Status;
    }
}

/// Abstract class for "somewhere".
class Location
{
    protected:
    Glib::ustring _name; ///<What we shall call this place.
    ///@deprecated: I'm not sure what to do with this, but it seems wrong.
    bool can_hunt; ///<Can we hunt here?

    public:
    Location(const Glib::ustring& name) : _name(name) {};
    /// Returns a friendly name for this place.
    Glib::ustring get_name () const {return _name;};
    virtual bool operator == (const Glib::ustring& rhs) const;
    /// Does something with the party while they're here.
    virtual Freetrail::Runner::Status run (Party* party) const = 0;

#ifndef NDEBUG
    friend std::ostream& operator <<(std::ostream& os, const Location& loc);
#endif
};

/// A respectable, safe place to stop and shop.
class Outpost : public Location
{
    public:
    Outpost (const Glib::ustring& name);
    /// Allows the party to rest and purchase supplies.
    Freetrail::Runner::Status run (Party* party) const;
};

/// A road to travel, the most common type of location.
class Path : public Location
{
    unsigned int _next_distance; ///<Distance to the next Location

    public:
    Path (const Glib::ustring& name, const unsigned int distance) :
        Location(name), _next_distance(distance) {};
    /// How far away is the next location, from start to finish.
    unsigned int get_next_distance () const {return _next_distance;};
    /// Should only be called by map parser.
    void set_next_location(Location* next_location);
    /// Moves a party step by step through a path, possibly over several turns.
    Freetrail::Runner::Status run (Party* party) const;
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

typedef std::vector<ForkOption*> ForkOptionContainer;
/// A classic fork in the road.
class Fork : public Location
{
    const ForkOptionContainer _jump_locations; ///<Possible places we could go.

    public:
    Fork (const Glib::ustring& name,
          const ForkOptionContainer& jump_locations) :
        Location(name), _jump_locations(jump_locations)
    {};
    ~Fork();

    /// Allows the user to make a decision of where to go.
    Freetrail::Runner::Status run (Party* party) const;
    /// Returns possible locations to go to.
    const ForkOptionContainer& get_jumps () const
        {return _jump_locations;};
};

#endif // LOCATION_H
