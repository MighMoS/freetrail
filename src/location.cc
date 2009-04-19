#include <cassert>
#include <iostream>

#include <glibmm.h>

#include "location.hh"
#include "party.hh"
#include "runners.hh"
#include "ui.hh"

bool CostsMoney::canPay (const Party& party)
{
    return party.get_money () > _amt;
}

/**
 *@param rhs a Glib::ustring to compare this Location to.
 */
bool Location::operator == (const Glib::ustring& rhs) const
{
    return _name == rhs;
}

bool ForkOption::canPay (const Party& party) const
{
    for (CostContainer::const_iterator i = _cost.begin();
            i != _cost.end (); i++)
    {
        if (!(*i)->canPay (party))
            return false;
    }
    return true;
}

/**
 *@param name circumstances requiring a fork
 *@param jump_locations a list of choices for the user
 */
Fork::Fork (const Glib::ustring& name,
        const ForkOptionContainer& jump_locations):
    Location (name), _jump_locations (jump_locations)
{};

/**
 *@param[in,out] party unused.
 */
Freetrail::Runner::Status Fork::run (Party& party) const
{
    Freetrail::Runner::Status stat;
    Glib::ustring destination;

    stat.setNextTrack (user_interface::getForkChoice (*this).get_destination());

    return stat;
}

/**
 *@param name circumstances requiring this jump
 *@param jump_locations where we're going.
 */
FixedJump::FixedJump (const Glib::ustring& name,
        const ForkOptionContainer& jump_locations):
    Fork (name, jump_locations)
{};

/**
 *@param party unused
 */
Freetrail::Runner::Status FixedJump::run (Party& party) const
{
    Freetrail::Runner::Status stat;
    stat.setNextTrack ((*get_jumps().begin ())->get_destination ());

    return stat;
}

/**
 *@param[in,out] party Party to subject to this Location.
 */
Freetrail::Runner::Status Path::run (Party& party) const
{
    Freetrail::Runner::Status stat;
    unsigned int speed = party.get_speed();
    unsigned int distance_traveled = 0;
    unsigned int members_left;
    bool reached_landmark = false;

    while (reached_landmark == false)
    {
        if (_next_distance <= speed + distance_traveled)
        {
            speed = _next_distance - distance_traveled;
            reached_landmark = true;
        }

        distance_traveled += speed;

        user_interface::displayStarvedMembers (party.eat());
        members_left = party.size();

        if (members_left == 0)
        {
            stat.setStatus (Freetrail::Runner::LOSE);
            return stat;
        }

        user_interface::travel_path_summery (party, speed, distance_traveled);
    }

    user_interface::travel_path_finish (get_name ());

    return stat;
}

/**
 *@see Path::run ()
 */
Freetrail::Runner::Status WinningPath::run (Party& party) const
{
    Freetrail::Runner::Status stat;

    stat = Path::run (party);
    // Actually, don't keep running, we're done!
    if (stat.KeepRunning ())
        stat.setStatus (Freetrail::Runner::WIN);

    return stat;
}

#ifndef NDEBUG
std::ostream& operator << (std::ostream& os, const Location& loc)
{
    os << "\n  Name: " << loc.get_name ();
    return os;
}
#endif

/**
 *@param name what this outpost is called.
 */
Outpost::Outpost (const Glib::ustring& name) :
    Location (name)
{}

/**
 *@param[in,out] party Party to subject to this Location.
 */
Freetrail::Runner::Status Outpost::run (Party& party) const
{
    user_interface::shop (party);
    return Freetrail::Runner::Status();
}
