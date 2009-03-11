#include <iostream>
#include <vector>

#include "common.hh"
#include "location.hh"
#include "ui.hh"

/**
 * Waits the user to press a key.
 * This is used to give the illusion of ``slowly progressing''.
 */
void user_interface::wait_for_key()
{
    char ch;
    std::cout << "\nPress a key to continue...\n";
    std::cin.get(ch);
    std::cout << horizrule;
}

/**
 * @param loc The Fork which we will draw our choices from.
 * @bug Doesn't do input validation.
 */
const ForkOption* user_interface::prompt_at_fork (const Fork& loc)
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
    std::cin >> counter;
    return fork_vec[counter-1].get (); // people count from 1, we don't.
}

/**
 * @bug This is braindead and dumb (IE: it does't actually clear).
 */
void user_interface::clear_screen()
{
    for (int i = 0; i < 24; i++)
        std::cout << "\n";
    std::cout << horizrule;
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

void user_interface::welcome_new_user()
{
    clear_screen();
    std::cout << "Welcome to FreeTrail. Adventure and fortune awaits you if you "
        "can survive.\n"
        "You set out to gather your party and resources...\n";
    wait_for_key();
}

void user_interface::win(const Party& party)
{
    std::cout << "Congratulations, you've won with " <<
        party.get_members()->size() << " members remaining!\n";
    wait_for_key();
}
