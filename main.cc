#include <iostream>

#include "party.hh"

using std::cin;
using std::cout;
using std::endl;

void main_loop ();
party* init_recruit ();

int main (int argc, char **argv)
{
	party* the_party;
	the_party = init_recruit ();
	main_loop();
	return 0;
}

void main_loop ()
{
	//cout << "Please pick an option:\n";
}

party* init_recruit ()
{
	party* some_party;
	some_party->init_party();
	return some_party;
}
