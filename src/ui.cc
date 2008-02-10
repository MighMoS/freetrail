#include <iostream>
#include "ui.hh"

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

