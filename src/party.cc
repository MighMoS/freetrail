#include <cassert>

#include <glibmm.h>

#include "party.hh"
#include "ui.hh"

unsigned int Health::feed ()
{
    assert (_hunger != 0);
    if (_hunger < 7)
        _hunger++;

    assert (_hunger <= 7);

    return _hunger;
}

unsigned int Health::starve ()
{
    assert (_is_alive == true);

    _hunger--;
    if (_hunger == 0)
    {
        _is_alive = false;
    }

    return _hunger;
}

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

unsigned int Member::starve ()
{
    unsigned int health;
    health = _health.starve ();
    user_interface::starving_member (*this);

    return health;
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

static bool compare_two_members_hunger (const Member&lhs,
        const Member& rhs)
{
    return lhs.get_hunger () < rhs.get_hunger ();
}
/**
 *
 */
int Party::eat_food ()
{
    int food_eaten;
    std::sort (_members.begin (), _members.end (),
            compare_two_members_hunger);
    for (MemberContainer::iterator i = _members.begin ();
            i != _members.end (); i++)
    {
        if (_food >= 5)
        {
            _food -= 5;
            food_eaten += 5;
            i->feed ();
        }
        else
        {
            i->starve ();
        }
    }

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
