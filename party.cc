#include <iostream>
using std::cout;
using std::cin;
using std::endl;

#include "party.hh"
#include "common.hh"

void party::init_party ()
{
	bool is_done = false;
	short choice = 0;
	sex_ their_sex;
	string name;

	do 
	{
	cout << "\nIs your party member\n1) Male\n2) Female\n";
	cout << select_one;
	cin >> choice;
	if (choice  == 1)
		their_sex = MALE;
	else
		their_sex = FEMALE;
	cout << "What is " << (their_sex == MALE ? "his " : "her ") << "name? ";
	cin >> name;

	members.push_back (member(their_sex, name));
	cout << "\nDo you wish to have another member?\n1) Yes\n2) No\n";
	cout << select_one;
	cin >> choice;
	}
	while (choice == 1);

	vector<member>::iterator iter;
	cout << "\nYou have the following people in your party:";
	for (iter = members.begin(); iter != members.end(); iter++)
	{
		cout << "\n\t" << iter->get_name();
	}
	cout << endl;
}

