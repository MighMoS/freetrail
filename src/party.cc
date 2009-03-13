#include <glibmm.h>

#include "party.hh"

Party::Party (const MemberContainer& members) :
    _members (members), _food(100), _ammo (50), _oxen (1), _money (1000)
{}

const MemberContainer* Party::get_members() const
{
    return &_members;
}

/* In the future this will calculate speed base on some formula
 *   I'm thinking (oxen * 15)-(weight * 0.3), or something of that nature
 * Right now, just travel us oxen * 15
 */
unsigned int Party::get_speed() const
{
    return _oxen * 15;
}

Member::Member (const sex its_sex, const Glib::ustring& name)
{
        _sex = its_sex;
        _name = name;
}

const Glib::ustring& Member::get_name () const
{
    return _name;
}

unsigned int Party::buy_ammo (const unsigned int amount)
{
    _money -= amount;
    return _ammo += 10;
}

unsigned int Party::buy_food (const unsigned int amount)
{
    _money -= amount;
    return _food += 50;
}

unsigned int Party::buy_oxen (const unsigned int amount)
{
    _money -= amount;
    return ++_oxen;
}

/**
 *@bug Currently there is no penalty if people can't eat.
 */
int Party::eat_food ()
{
    int food_eaten;
    food_eaten = 3 * _members.size ();
    // Don't descend into negatives
    if (_food - food_eaten < 0)
    {
        _food = 0;
        // XXX If we need to eat 20 lbs and only have 2, this should be penalized.
        food_eaten = _food;
    }
    _food -= food_eaten;

    return food_eaten;
}

unsigned int Party::get_ammo () const
{
    return _ammo;
}

int Party::get_food () const
{
    return _food;
}

unsigned int Party::get_money () const
{
    return _money;
}

unsigned int Party::get_oxen () const
{
    return _oxen;
}

