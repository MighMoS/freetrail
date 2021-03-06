#ifndef LOCATION_H
#define LOCATION_H

#ifndef NDEBUG
#include <iostream>
#endif
#include <tr1/memory>

#include <glibmm.h>
#include <vector>

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
    Location(const Glib::ustring& name) : _name(name), can_hunt(false) {};
    /// Returns a friendly name for this place.
    Glib::ustring get_name () const {return _name;};
    virtual bool operator == (const Glib::ustring& rhs) const;
    /// Does something with the party while they're here.
    virtual Freetrail::Runner::Status run (Party& party) const = 0;

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
    Freetrail::Runner::Status run (Party& party) const;
};

/// A road to travel, the most common type of location.
class Path : public Location
{
    unsigned int _next_distance; ///<Distance to the next Location

    public:
    Path (const Glib::ustring& name, const unsigned int distance) :
        Location(name), _next_distance(distance) {};
    /// How far away is the next location, from start to finish.
    unsigned int length () const {return _next_distance;};
    /// Moves a party step by step through a path, possibly over several turns.
    Freetrail::Runner::Status run (Party& party) const;
};

/// Completing this Path will win the game for the user.
class WinningPath : public Path
{
    public:
    WinningPath (const Glib::ustring& name, const unsigned int distance) :
        Path (name, distance) {};
    /// Functions just like Path::run, but win if we get through it.
    Freetrail::Runner::Status run (Party& party) const;
};

class Cost
{
    protected:
    const unsigned int _amt;
    const Glib::ustring _description;
    public:
    Cost (unsigned int amt, const Glib::ustring& description)
        : _amt (amt), _description(description) {};
    virtual bool canPay (const Party& party) const = 0;
    const Glib::ustring& description () const {return _description;};
};

class CostsMoney : public Cost
{
    public:
    CostsMoney (unsigned int amt) : Cost (amt, "dollars") {};
    bool canPay (const Party & party) const;
};

typedef std::tr1::shared_ptr<Cost> CostPtr;
typedef std::vector<CostPtr> CostContainer;
/// Container object holding a location, and a "how to get there".
class ForkOption
{
    const Glib::ustring _description; ///<A friendly description of what's here.
    const Glib::ustring _destination; ///< Name of the destination track.
    const CostContainer _cost;

    public:
    ForkOption (const Glib::ustring& desc, const Glib::ustring& dest,
            const CostContainer& cost) :
        _description(desc), _destination(dest), _cost (cost)
    {};
    /// Returns what's special about this path
    const Glib::ustring& get_description () const {return _description;};
    /// Returns an int pointing to the track to jump to
    const Glib::ustring& get_destination () const {return _destination;};
    bool canPay (const Party& party) const;
};

typedef std::tr1::shared_ptr<ForkOption> ForkOptionPtr;
typedef std::vector<ForkOptionPtr> ForkOptionContainer;
/// A classic fork in the road.
class Fork : public Location
{
    const ForkOptionContainer _jump_locations; ///<Possible places we could go.

    public:
    Fork (const Glib::ustring& name,
          const ForkOptionContainer& jump_locations);

    /// Allows the user to make a decision of where to go.
    Freetrail::Runner::Status run (Party& party) const;
    /// Returns possible locations to go to.
    const ForkOptionContainer& get_jumps () const
        {return _jump_locations;};
};

/// A FixedJump occurs at the end of a Track to automatically jump us somewhere
class FixedJump : public Fork
{
    public:
    FixedJump (const Glib::ustring& name,
          const ForkOptionContainer& jump_locations);
    /// Jump somewhere, no questions asked.
    Freetrail::Runner::Status run (Party& party) const;
};

#endif // LOCATION_H
