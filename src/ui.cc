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

void user_interface::welcome_new_user()
{
    clear_screen();
    std::cout << "Welcome to FreeTrail. Adventure and fortune awaits you if you "
        "can survive.\n"
        "You set out to gather your party and resources...\n";
    wait_for_key();
}
