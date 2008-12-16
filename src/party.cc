#include <iostream>
using std::cout;
using std::cin;
using std::endl;

#include "party.hh"
#include "common.hh"

const int PRICE_OF_OXEN = 175;
const int PRICE_OF_FOOD = 25;
const int PRICE_OF_AMMO = 10;

Party::Party () : food(100), ammo (50), oxen (0), money (1000), 
    distance_travelled (0)
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
    shop();
}

/* Offers the party a chance to buy stuffs. 
 */
void Party::shop()
{
    short choice;
    do
    {
        cout << "\nWhat would you like to buy?\n" <<
            "1) Oxen ($" << PRICE_OF_OXEN <<
                ")\t\t (You have " << oxen << ")\n" <<
            "2) Food ($" << PRICE_OF_FOOD << " / 50lb)" <<
                "\t (You have " << food << "lbs)\n" <<
            "3) Ammo ($" << PRICE_OF_AMMO << " / 10lb)" <<
                "\t (You have " << ammo << ")\n" <<
            "0) Done\n";
        cout << "\nYou have $" << money << endl;
        cout << select_one;
        cin >> choice;
        switch (choice)
        {
            case 1:
            money -= PRICE_OF_OXEN; oxen++; break;
            case 2:
            money -= PRICE_OF_FOOD; food += 50; break;
            case 3:
            money -= PRICE_OF_AMMO; ammo += 10;
        }
    } //do ... while()
    while (choice);
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

// TODO: Make people hungry if they can't eat.
unsigned int Party::eat_food ()
{
    int food_eaten;
    food_eaten = 3 * members.size();
    // Don't descend into negatives, etc.
    if (food - food_eaten < 0)
    {
        food = 0;
        // XXX If we need to eat 20 lbs and only have 2, this shoudl be penalized.
        food_eaten = food;
    }
    food -= food_eaten;

    return food_eaten;
}

unsigned int Party::get_food () const
{
    return food;
}

// Probably shouldn't directly take a map
// TODO: Write out a better way.
void Party::reached_landmark (const Map* map, const unsigned int track_no)
{
    distance_travelled = 0;

    // Sizes won't be zero based, subtract 1
    if (map->get_track_size(track_no) - 1 <= track_position)
    {
        track_position = 0;
        if (map->get_number_tracks() - 1 <= track_number)
            track_number++;
        else
            ; // Signify winning, but this should happen elsewhere. Hmmm...
    }
    else
    {
        track_position++;
    }
}
