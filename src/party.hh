#ifndef PARTY
#define PARTY

#include <string>
using std::string;
#include <vector>
using std::vector;

#include "common.hh"
#include "world.hh"

class Member
{
	private:
		sex_ sex;
		health_ health;
		int hunting_skill;
		string name;

	public:
		sex_ get_sex () const;
		health_ get_health () const;
		int get_hunting_skill () const;
		string get_name () const;
		Member (const sex_ its_sex, const string& its_name);
};

class Party
{
	vector<Member> members;
	int food; // in LB
	int ammo; // in shots
	int oxen;
	int money;
	int distance_travelled;
	int speed;

	public:
	int get_speed () const;
	int get_distance () const;
	unsigned int get_food () const;
	unsigned int eat_food ();
	void add_distance (const int mileage);
	void init_party ();
	void shop ();

	Party();
};
#endif 
