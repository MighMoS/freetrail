#include <iostream>

#include "location.hh"
#include "party.hh"
#include "runners.hh"
#include "ui.hh"

bool Location::operator == (const Glib::ustring& rhs) const
{
    return _name == rhs;
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

Freetrail::Runner::Status Outpost::run (Party* party)
{
    return Freetrail::Runner::Status();
}
