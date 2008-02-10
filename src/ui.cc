#include <iostream>
#include "ui.hh"

/* Waits the user to press a key. This is used to give the illusion of 
 * ``slowly progressing''.
 */
void user_interface::wait_for_key()
{
	char ch;
	std::cout << "\nPress a key to continue...\n";
	std::cin.get(ch);
}

//TODO: replace this with a /real/ clear screen function
void user_interface::clear_screen()
{
	for (int i = 0; i < 24; i++)
		std::cout << "\n";
}

