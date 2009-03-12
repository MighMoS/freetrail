#include <cassert>
#include <iostream>
using std::cout;
using std::cin;
using std::endl;

#include <glibmm.h>

#include "common.hh"
#include "party.hh"
#include "ui.hh"

Party::Party () : _food(100), _ammo (50), _oxen (1), _money (1000)
{}

/* Initializes the party with members.
 * The user is prompted for player's sex and names, and they are added.
 * Once the user is done, we confirm to make sure
 * XXX: Presently shop() is called, ideally we would call this elsewhere
 */
void Party::init_party ()
{
    short choice = 0;
    sex their_sex;
    std::string name;

    cout << "Who do you want in your party?\n";
add_members: do 
    {
        cout << "\nIs your party member\n1) Male\n2) Female\n";
        cout << select_one;
        cin >> choice;
        if (choice == 1)
            their_sex = MALE;
        else
            their_sex = FEMALE;

        cin.ignore(); // Needed to avoid getline screwing up

        cout << "What is " << (their_sex == MALE ? "his " : "her ") << "name? ";
        getline (cin, name);

        _members.push_back (Member(their_sex, name));
        cout << "\nDo you wish to have another member?\n1) Yes\n2) No\n";
        cout << select_one;
        cin >> choice;
    }
    while (choice == 1);
    cout << horizrule;

    MemberContainer::iterator iter;
    cout << "You have the following people in your party:\n";
    for (iter = _members.begin(); iter != _members.end(); iter++)
        cout << "\n\t" << iter->get_name();
    cout << "\n\nAre you sure this is it?\n1) Yes\n2) No\n";
    cout << select_one;
    cin >> choice;
    if (choice == 2)
        goto add_members; // Goto avoids horrible nested logic. 
    cout << horizrule;
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

// TODO: Make people hungry if they can't eat.
int Party::eat_food ()
{
    int food_eaten;
    food_eaten = 3 * _members.size();
    // Don't descend into negatives, etc.
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

