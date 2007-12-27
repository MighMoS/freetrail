#include <iostream>
using std::cout;
using std::cin;
using std::endl;

#include "party.hh"

void party::init_party ()
{
	bool is_done = false;
	short choice = 0;
	sex_ their_sex;
	string name;

	cout << "Is your party member\n1) Male\n2) Female\n";
	cin >> choice;
	if (choice  == 1)
		their_sex = MALE;
	else
		their_sex = FEMALE;
	cout << "What is " << (their_sex == MALE ? "his " : "her ") << "name? ";
	cin >> name;

	members.push_back (member(their_sex, name));
}

