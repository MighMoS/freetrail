#include <cassert>
#include <iostream>

#include <glibmm.h>

#include "common.hh"
#include "location.hh"
#include "party.hh"
#include "runners.hh"
#include "ui.hh"

/**
 *@param rhs a Glib::ustring to compare this Location to.
 */
bool Location::operator == (const Glib::ustring& rhs) const
{
    return _name == rhs;
}

/**
 *@param name circumstances requiring a fork
 *@param jump_locations a list of choices for the user
 */
Fork::Fork (const Glib::ustring& name,
        const ForkOptionContainer& jump_locations):
    Location(name), _jump_locations(jump_locations)
{};

/**
 *@param[in,out] party Party to ask where to go.
 */
Freetrail::Runner::Status Fork::run (Party* party) const
{
    Freetrail::Runner::Status stat;
    Glib::ustring destination;
    const ForkOption* chosen;

    chosen = user_interface::prompt_at_fork (*this);
    destination = chosen->get_destination ();
    stat.setNextTrack (destination);

    return stat;
}

/**
 *@param name circumstances requiring this jump
 *@param jump_locations where we're going.
 */
FixedJump::FixedJump (const Glib::ustring& name,
        const ForkOptionContainer& jump_locations):
    Fork(name, jump_locations)
{};

/**
 *@param party unused
 */
Freetrail::Runner::Status FixedJump::run (Party* party) const
{
    Freetrail::Runner::Status stat;
    stat.setNextTrack ((*get_jumps().begin ())->get_destination ());

    return stat;
}

/**
 *@param[in,out] party Party to subject to this Location.
 */
Freetrail::Runner::Status Path::run(Party* party) const
{
    Freetrail::Runner::Status stat;
    const unsigned int speed = party->get_speed();
    unsigned int distance_traveled = 0;
    unsigned int food_eaten;
    bool reached_landmark = false;

    while (reached_landmark == false)
    {
        if (get_next_distance() > speed + distance_traveled)
        {
            distance_traveled += speed;
        }
        else
        {
            distance_traveled = get_next_distance() - distance_traveled;
            reached_landmark = true;
        }
        food_eaten = party->eat_food();

        std::cout << "You traveled " << distance_traveled << " miles today and have "
            << party->get_food() << " lbs of food remaining.\n";
        user_interface::wait_for_key();
    }

    std::cout << horizrule;
    std::cout << "You've arrived at " << get_name() << std::endl;
    user_interface::wait_for_key();

    return stat;
}

/**
 *@see Path::run ()
 */
Freetrail::Runner::Status WinningPath::run (Party* party) const
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
 *@param[in,out] name what this outpost is called.
 */
Outpost::Outpost (const Glib::ustring& name) :
    Location (name)
{}

/**
 *@param[in,out] party Party to subject to this Location.
 */
Freetrail::Runner::Status Outpost::run (Party* party) const
{
    user_interface::shop(*party);
    return Freetrail::Runner::Status();
}
