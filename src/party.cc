#include <algorithm>
#include <cassert>
#include <memory>

#include <glibmm.h>

#include "party.hh"
#include "ui.hh"

/**
 *@returns how hungry the party member is now.
 */
unsigned int Health::feed ()
{
    assert (_hunger != 0);
    if (_hunger < 7)
        _hunger++;

    assert (_hunger <= 7);

    return _hunger;
}

/**
 *@returns how hungry the party member is now.
 *@note this may kill the member.
 */
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

/**
 *@param members Fully initialized MemberContainer
 */
Party::Party (const MemberContainer& members) :
    _members (members), _food (100), _ammo (50), _oxen (1), _money (1000)
{}

/**
 *@param member a fully initialized member who will join the party.
 */
void Party::add_member (const Member& member)
{
    _members.insert (_members.begin (), member);
}

const MemberContainer& Party::get_members () const
{
    return _members;
}

/**
 * In the future this will calculate speed base on some formula
 *   I'm thinking (oxen * 15)-(weight * 0.3), or something of that nature
 * Right now, just travel us oxen * 15
 */
unsigned int Party::get_speed() const
{
    return _oxen * 15;
}

/**
 *@param name The unique name of this party member, may contain spaces.
 *@param its_sex girl or boy?
 */
Member::Member (const Glib::ustring& name, const sex its_sex) :
    _name (name), _sex (its_sex)
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
 */
MemberContainerPtr Party::get_active_members () const
{
    MemberContainerPtr active_members (new MemberContainer);
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
 */
MemberContainerPtr Party::get_inactive_members () const
{
    MemberContainerPtr inactive_members (new MemberContainer);
    MemberContainer::const_iterator i
        = std::find_if (_members.begin (), _members.end (),
                is_member_not_alive);
    while (i != _members.end ())
    {
        inactive_members->insert (inactive_members->begin (), *i);
        std::advance (i, 1); // Skip over ourselves, or we'll loop.
        i = std::find_if (i, _members.end (), is_member_not_alive);
    }

    return inactive_members;
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

/**
 *@returns A MemberConatiner of every party member which couldn't eat.
 */
MemberContainer Party::eat ()
{
    MemberContainer new_members;
    MemberContainer starved_members;
    MemberContainerPtr active_members (get_active_members ());
    MemberContainerPtr inactive_members (get_inactive_members ());
    MemberContainer::iterator i = active_members->begin ();

    while (i != active_members->end ())
    {
        Member curr_member = *i;
        if (_food >= 5)
        {
            _food -= 5;
            curr_member.feed ();
        }
        else
        {
            curr_member.starve ();
            starved_members.insert (curr_member);
        }
        MemberContainer::iterator e = i;
        i++;
        active_members->erase (e);
        active_members->insert (active_members->begin (), curr_member);
    }

    // Recombine our updated members with the others we didn't
    std::set_union (active_members->begin (), active_members->end (),
            inactive_members->begin (), inactive_members->end (),
            std::inserter(new_members, new_members.begin ()));
    _members.swap (new_members);

    return starved_members;
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

unsigned int Party::size () const
{
    return get_active_members ()->size ();
}
