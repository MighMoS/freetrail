#include <iostream>

#include "location.hh"
#include "party.hh"
#include "runners.hh"
#include "ui.hh"

///Set the next location
void Path::set_next_location(Location* next_location)
{
    _next_location = next_location;
#ifndef NDEBUG
    std::cout << _next_location << std::endl;
#endif
}

///Returns the next location* in line.
/**
 * @bug Assumes that a Path will never be the last location in a Track
 */
std::vector<Location*>
Path::getNext (const unsigned int track_no, const unsigned int position) const
{
    std::vector<location*> next_location;

    next_location.push_back(_next_location);

    return next_location;
}

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

Outpost::Outpost (const Glib::ustring& name) :
    Location (name)
{}
