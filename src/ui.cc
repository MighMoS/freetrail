#include <iostream>
#include <utility>

#include <glibmm.h>

#include "location.hh"
#include "parser.hh"
#include "party.hh"
#include "ui.hh"
#include "world.hh"

/**
 * @bug This is braindead and dumb (IE: it does't actually clear).
 */
void user_interface::clear_screen()
{
    for (int i = 0; i < 24; i++)
        std::cout << "\n";
    std::cout << horizrule;
}

void user_interface::lose ()
{
    std::cout << "You've lose. Better luck next time.\n";
    wait_for_key ();
}

/**
 *@returns a full, ready to use Party.
 *@todo check to see if the member already exists, then handle gracefully.
 */
Party user_interface::createParty ()
{
    // The following just opens up stdin for a convienience function
    Glib::RefPtr<Glib::IOChannel> Gcin (Glib::IOChannel::create_from_fd(0));
    unsigned short choice;
    MemberContainer members;

    std::cout << "Who do you want in your party?\n";

add_members:
    do
    {
        Glib::ustring name;
        std::pair<MemberContainer::iterator,bool> ret;
        sex their_sex;

        std::cout << "\nIs your party member\n1) Male\n2) Female\n";
        std::cout << select_one;
        std::cin >> choice;
        if (choice == 1)
            their_sex = MALE;
        else
            their_sex = FEMALE;

        std::cout << "What is " << (their_sex == MALE ? "his " : "her ")
            << "name? " << std::flush;
        Gcin->read_line (name);
        name.erase (name.find ('\n'));

        ret = members.insert (Member(name, their_sex));
        std::cout << "\nDo you wish to have another member?\n1) Yes\n2) No\n";
        std::cout << select_one;
        std::cin >> choice;
    }
    while (choice == 1);

    std::cout << horizrule;

    MemberContainer::const_iterator iter;
    std::cout << "You have the following people in your party:\n";
    for (iter = members.begin (); iter != members.end (); iter++)
        std::cout << "\n\t" << iter->get_name();
    std::cout << "\n\nAre you sure this is it?\n1) Yes\n2) No\n";
    std::cout << select_one;
    std::cin >> choice;
    if (choice == 2)
        goto add_members; // avoid horrible nested logic and indentation.
    std::cout << horizrule;

    return Party (members);
}

/**
 * @param loc The Fork which we will draw our choices from.
 */
const ForkOption& user_interface::getForkChoice (const Fork& loc)
{
    ForkOptionContainer fork_vec = loc.get_jumps ();
    unsigned int counter = 0; ///<Which number are we displaying? What does the user want?

    std::cout << "You've arrived at " << loc.get_name () << "\n";
    std::cout << "It would appear that there are " << fork_vec.size() << " options.\n";
    std::cout << "Where should we go?\n";
    for (ForkOptionContainer::iterator i = fork_vec.begin();
            i != fork_vec.end(); i++)
    {
        counter++;
        std::cout << counter << ") " << (*i)->get_description() << "\n";
    }
    do
    {
        std::cin >> counter;
    } while (counter > fork_vec.size ());

    return *fork_vec[counter-1]; // people count from 1, we don't.
}

/**
 *@todo actually ask the user what they want to load
 *@todo should we recurse if we can't load the map?
 */
const Map user_interface::getMapChoice ()
{
    try
    {
        return MapParser("example.xml").parse ();
    }
    catch (const MapParsingException& e)
    {
        std::cerr << "ERROR LOADING THE MAP:\n\t";
        std::cerr << e.what() << std::endl;
        return getMapChoice ();
    }
}
/**
 * @param[in,out] party the party to purchase supplies.
 */
void user_interface::shop(Party& party)
{
    const int PRICE_OF_OXEN = 175;
    const int PRICE_OF_FOOD = 25;
    const int PRICE_OF_AMMO = 10;

    unsigned int amt_ammo = party.get_ammo();
    unsigned int amt_food = party.get_food();
    unsigned int amt_oxen = party.get_oxen();

    short choice;
    do
    {
        clear_screen();
        std::cout << "\nWhat would you like to buy?\n" <<
            "1) Oxen ($" << PRICE_OF_OXEN <<
                ")\t\t (You have " << amt_oxen << ")\n" <<
            "2) Food ($" << PRICE_OF_FOOD << " / 50lb)" <<
                "\t (You have " << amt_food << "lbs)\n" <<
            "3) Ammo ($" << PRICE_OF_AMMO << " / 10lb)" <<
                "\t (You have " << amt_ammo << ")\n" <<
            "0) Done\n";
        std::cout << "\nYou have $" << party.get_money() << std::endl;
        std::cout << select_one;
        std::cin >> choice;
        switch (choice)
        {
            case 1:
            amt_oxen = party.buy_oxen(PRICE_OF_OXEN); break;
            case 2:
            amt_food = party.buy_food(PRICE_OF_FOOD); break;
            case 3:
            amt_ammo = party.buy_ammo(PRICE_OF_AMMO);
        }
    } //do ... while()
    while (choice);
}

/**
 *@param member A party member who's whining we should tell the user about.
 */
static void starving_member (const Member& member)
{
    unsigned int hunger = member.get_hunger();
    if (hunger > 0)
        std::cout << member.get_name () << " couldn't eat today. (health "
            << hunger << "/7)\n";
    else
    {
        std::cout << member.get_name ()
            << " died because there wasn't enough food.\n";
    }
}

/**
 *@param hungry_members A container holding every member which couldn't eat.
 */
void
user_interface::displayStarvedMembers (const MemberContainer& hungry_members)
{
    for (MemberContainer::const_iterator i = hungry_members.begin ();
            i != hungry_members.end (); i++)
    {
        starving_member (*i);
    }
}

/**
 *@param where Where we've just gotten to after traveling down a long <s>road</s> Path
 */
void user_interface::travel_path_finish (const Glib::ustring& where)
{
    std::cout << horizrule;
    std::cout << "You've arrived at " << where << "\n";
    wait_for_key ();
}
/**
 *@param party the Party which is traveling
 *@param speed how far the party went `today'
 *@param total how far the party has gone so far.
 */
void user_interface::travel_path_summery (const Party& party,
        const unsigned int speed, const unsigned int total)
{
    std::cout << "You traveled " << speed << " miles today ("
        << total << " miles total) and have "
        << party.get_food () << " lbs of food remaining.\n";

    std::cout << "Your party consists of " << party.size() << " members.\n";
    wait_for_key ();
}

/**
 * Waits the user to press a key.
 * This is used to give the illusion of ``slowly progressing''.
 */
void user_interface::wait_for_key ()
{
    char ch;
    std::cout << "\nPress enter to continue...\n";
    std::cin.get (ch);
    std::cout << horizrule;
}

void user_interface::welcome_new_user()
{
    clear_screen ();
    std::cout << "Welcome to FreeTrail. Adventure and fortune awaits you if you "
        "can survive.\n"
        "You set out to gather your party and resources...\n";
    wait_for_key ();
}

void user_interface::win(const Party& party)
{
    std::cout << "Congratulations, you've won with " <<
        party.size () << " members remaining!\n";
    wait_for_key ();
}
