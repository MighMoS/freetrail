#include <iostream>
using std::cout;
using std::cin;
using std::endl;

#include "party.hh"
#include "common.hh"

const int PRICE_OF_OXEN = 175;
const int PRICE_OF_FOOD = 25;
const int PRICE_OF_AMMO = 10;

party::party () : food(100), ammo (50), oxen (0), money (1000), 
	distance_travelled (0)
{}

/* Initializes the party with members.
 * The user is prompted for player's sex and names, and they are added.
 * Once the user is done, we confirm to make sure
 * XXX: Presently shop() is called, ideally we would call this elsewhere
 */
void party::init_party ()
{
	short choice = 0;
	sex_ their_sex;
	string name;

	cout << "Who do you want in your party?\n";
add_members:
	do 
	{
	cout << "\nIs your party member\n1) Male\n2) Female\n";
	cout << select_one;
	cin >> choice;
	if (choice  == 1)
		their_sex = MALE;
	else
		their_sex = FEMALE;

	cin.ignore(); // Needed to avoid getline screwing up

	cout << "What is " << (their_sex == MALE ? "his " : "her ") << "name? ";
	getline (cin, name);

	members.push_back (member(their_sex, name));
	cout << "\nDo you wish to have another member?\n1) Yes\n2) No\n";
	cout << select_one;
	cin >> choice;
	}
	while (choice == 1);
	cout << horizrule;

	vector<member>::iterator iter;
	cout << "You have the following people in your party:\n";
	for (iter = members.begin(); iter != members.end(); iter++)
		cout << "\n\t" << iter->get_name();
	cout << "\n\nAre you sure this is it?\n1) Yes\n2) No\n";
	cout << select_one;
	cin >> choice;
	if (choice == 2)
		goto add_members; // Goto avoids horrible nested logic. 
	cout << horizrule;

	// We call shop first at init party
	shop();
}

/* Offers the party a chance to buy stuffs. 
 */
void party::shop()
{
	short choice;
	do
	{
	cout << "\nWhat would you like to buy?\n" <<
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
	} // switch
	} //do ... while()
	while (choice);
}

/* In the future this will calculate speed base on some formula
 *   I'm thinking (oxen * 15)-(weight * 0.3), or something of that nature
 * Right now, just travel us oxen * 15
 */
int party::get_speed() const
{
	return oxen * 15;
}

int party::get_distance() const
{
	return distance_travelled;
}

void party::add_distance (const int mileage)
{
	distance_travelled += mileage;
}

member::member (const sex_ its_sex, const string& its_name)
{
        sex = its_sex;
        name = its_name;
}

string member::get_name () const
{
	return name;
}

