#include <algorithm>
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
    assert (_hunger > 0);

    _hunger--;
    if (_hunger == 0)
    {
        _is_alive = false;
    }

    return _hunger;
}

Party::Party () :
    _food (100), _ammo (50), _oxen (1), _money (1000)
{}

Party::Party (const MemberContainer& members) :
    _members (members), _food (100), _ammo (50), _oxen (1), _money (1000)
{}

void Party::add_member (const Member& member)
{
    _members.insert (_members.begin (), member);
}

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

Member::Member (const sex its_sex, const Glib::ustring& name) :
    _sex (its_sex), _name (name)
{
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

static bool is_member_alive (const Member& lhs)
{
    return lhs.is_alive ();
}

static bool is_member_not_alive (const Member& lhs)
{
    return !lhs.is_alive ();
}

/// Gets all the party members who aren't dead.
/**
 *@returns A pointer to a new MemberContainer
 *@notes the caller is responsible for deleting the returned pointer
 */
MemberContainer* Party::get_active_members () const
{
    MemberContainer* active_members = new MemberContainer;
    MemberContainer::const_iterator i
        = std::find_if (_members.begin (), _members.end (), is_member_alive);
    while (i != _members.end ())
    {
        active_members->insert (active_members->begin (), *i);
        std::advance (i, 1); // Skip over ourselves, or we'll loop.
        i = std::find_if (i, _members.end (), is_member_alive);
    }

    return active_members;
}

/// Gets all the party members who aren't dead.
/**
 *@returns A pointer to a new MemberContainer
 *@notes the caller is responsible for deleting the returned pointer
 */
MemberContainer* Party::get_inactive_members () const
{
    MemberContainer* active_members = new MemberContainer;
    MemberContainer::const_iterator i
        = std::find_if (_members.begin (), _members.end (),
                is_member_not_alive);
    while (i != _members.end ())
    {
        active_members->insert (active_members->begin (), *i);
        std::advance (i, 1); // Skip over ourselves, or we'll loop.
        i = std::find_if (i, _members.end (), is_member_alive);
    }

    return active_members;
}

/// Returns which member is more hungry, followed by alphabetical ordering.
bool operator < (const Member& lhs, const Member& rhs)
{
    if (lhs.get_hunger () < rhs.get_hunger ())
        return  true;
    if (lhs.get_hunger () > rhs.get_hunger ())
        return false;

    // The two are equally hungry. Determine which one is more important the
    // way God meant: alphabetically
    // ~ Adam
    return lhs.get_name () < rhs.get_name ();
}

/// Consume food, and people get hungry if they can't eat. If they can't eat and are too hungry, they die.
/**
 *
 */
int Party::eat_food ()
{
    int food_eaten;
    MemberContainer* active_members = get_active_members ();
    MemberContainer* inactive_members = get_inactive_members ();
    MemberContainer new_members;

    for (MemberContainer::iterator i = active_members->begin ();
            i != active_members->end (); i++)
    {
        Member temp = *i;
        if (_food >= 5)
        {
            _food -= 5;
            food_eaten += 5;
            temp.feed ();
        }
        else
        {
            temp.starve ();
        }
        active_members->erase (i);
        active_members->insert (active_members->begin (), temp);
    }


    // Recombine our updated members with the others we didn't
    std::set_union (active_members->begin (), active_members->end (),
            inactive_members->begin (), inactive_members->end (),
            std::inserter(new_members, new_members.begin ()));
    _members.swap (new_members);

    delete active_members;
    delete inactive_members;

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
