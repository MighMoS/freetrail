#include <iostream>
using std::cout;
using std::cin;
using std::endl;

#include "party.hh"
#include "common.hh"

const int PRICE_OF_OXEN = 175;
const int PRICE_OF_FOOD = 25;
const int PRICE_OF_AMMO = 10;

party::party () : food(100), ammo (50), oxen (0), money (1000)
{}

void party::init_party ()
{
	short choice = 0;
	sex_ their_sex;
	string name;

	cout << "Who do you want in your party?\n";
	//Add people to the party
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

	do
	{
	cout << "\nWhat would you like to buy? (Press 0 to exit)\n" <<
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
		money -= PRICE_OF_AMMO; ammo += 10; break;
	}
	}
	while (choice);
	cout << "You begin your journey...\n";
}

