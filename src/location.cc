#include <iostream>

#include "location.hh"
#include "party.hh"
#include "runners.hh"
#include "ui.hh"

/**
 *@param[in] rhs a Glib::ustring to compare this Location to.
 */
bool Location::operator == (const Glib::ustring& rhs) const
{
    return _name == rhs;
}

/**
 *@param[in] party Party to subject to this Location.
 */
Freetrail::Runner::Status Fork::run (Party* party)
{
    Freetrail::Runner::Status stat;

    // Present options to the user.

    return stat;
}

/**
 *@param[in] party Party to subject to this Location.
 */
Freetrail::Runner::Status Path::run(Party* party)
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

#ifndef NDEBUG
std::ostream& operator << (std::ostream& os, const Location& loc)
{
    os << "\n  Name: " << loc.get_name ();
    return os;
}
#endif

/**
 *@param[in] name what this outpost is called.
 */
Outpost::Outpost (const Glib::ustring& name) :
    Location (name)
{}

/**
 *@param[in] party Party to subject to this Location.
 */
Freetrail::Runner::Status Outpost::run (Party* party)
{
    user_interface::shop(*party);
    return Freetrail::Runner::Status();
}
