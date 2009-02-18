#include <cassert>
#include <iostream>
using std::cout;
using std::cin;
using std::endl;

#include "common.hh"
#include "party.hh"
#include "ui.hh"

Party::Party () : food(100), ammo (50), oxen (0), money (1000), 
    distance_travelled (0), track_number(0), track_position(1)
{}

/* Initializes the party with members.
 * The user is prompted for player's sex and names, and they are added.
 * Once the user is done, we confirm to make sure
 * XXX: Presently shop() is called, ideally we would call this elsewhere
 */
void Party::init_party ()
{
    short choice = 0;
    sex_ their_sex;
    std::string name;

    cout << "Who do you want in your party?\n";
add_members: do 
    {
    cout << "\nIs your party member\n1) Male\n2) Female\n";
    cout << select_one;
    cin >> choice;
    if (choice  == 1)
        their_sex = MALE;
    else
        their_sex = FEMALE;

    cin.ignore(); // Needed to avoid getline screwing up

    cout << "What is " << (their_sex == MALE ? "his " : "her ") << "name? ";
    getline (cin, name);

    members.push_back (Member(their_sex, name));
    cout << "\nDo you wish to have another member?\n1) Yes\n2) No\n";
    cout << select_one;
    cin >> choice;
    }
    while (choice == 1);
    cout << horizrule;

    std::vector<Member>::iterator iter;
    cout << "You have the following people in your party:\n";
    for (iter = members.begin(); iter != members.end(); iter++)
        cout << "\n\t" << iter->get_name();
    cout << "\n\nAre you sure this is it?\n1) Yes\n2) No\n";
    cout << select_one;
    cin >> choice;
    if (choice == 2)
        goto add_members; // Goto avoids horrible nested logic. 
    cout << horizrule;

    // We call shop first at init party
    user_interface::shop(*this);
}

const std::vector<Member>* Party::get_members() const
{
    return &members;
}

/* In the future this will calculate speed base on some formula
 *   I'm thinking (oxen * 15)-(weight * 0.3), or something of that nature
 * Right now, just travel us oxen * 15
 */
int Party::get_speed() const
{
    return oxen * 15;
}

int Party::get_distance() const
{
    return distance_travelled;
}

void Party::add_distance (const int mileage)
{
    distance_travelled += mileage;
}

Member::Member (const sex_ its_sex, const std::string& its_name)
{
        sex = its_sex;
        name = its_name;
}

std::string Member::get_name () const
{
    return name;
}

unsigned int Party::buy_ammo (const unsigned int amount)
{
    money -= amount;
    return ammo += 10;
}

unsigned int Party::buy_food (const unsigned int amount)
{
    money -= amount;
    return food += 50;
}

unsigned int Party::buy_oxen (const unsigned int amount)
{
    money -= amount;
    return ++oxen;
}

// TODO: Make people hungry if they can't eat.
unsigned int Party::eat_food ()
{
    int food_eaten;
    food_eaten = 3 * members.size();
    // Don't descend into negatives, etc.
    if (food - food_eaten < 0)
    {
        food = 0;
        // XXX If we need to eat 20 lbs and only have 2, this should be penalized.
        food_eaten = food;
    }
    food -= food_eaten;

    return food_eaten;
}

unsigned int Party::get_ammo () const
{
    return ammo;
}

unsigned int Party::get_food () const
{
    return food;
}

unsigned int Party::get_money () const
{
    return money;
}

unsigned int Party::get_oxen () const
{
    return oxen;
}

// Probably shouldn't directly take a map
// TODO: Use a map/track iterator, which needs to be written.
// Returns false as long as there is still more map
//
// Returns TRUE if we're at the end -- we won!
bool Party::reached_landmark (const Map* map, const unsigned int track_no)
{
    distance_travelled = 0;

#if 0
    // Sizes won't be zero based, subtract 1
    if (map->get_track_size(track_no) - 1 <= track_position)
    {
        track_position = 0;
        assert (track_number <= map->get_number_tracks() - 1);
        track_number++;
    }
    else
    {
        track_position++;
    }

    // If there's no where to go, we've won.
    if (map->get_track(track_no)-> get_stop(track_position)-> get_next_distance() == 0)
        return true;
#endif

    return false;
}
