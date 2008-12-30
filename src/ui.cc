#include <iostream>

#include "common.hh"
#include "ui.hh"

/* Waits the user to press a key. This is used to give the illusion of 
 * ``slowly progressing''.
 */
void user_interface::wait_for_key()
{
    char ch;
    std::cout << "\nPress a key to continue...\n";
    std::cin.get(ch);
    std::cout << horizrule;
}

//TODO: replace this with a /real/ clear screen function
void user_interface::clear_screen()
{
    for (int i = 0; i < 24; i++)
        std::cout << "\n";
    std::cout << horizrule;
}

/* Offers the party a chance to buy stuffs.
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
