#ifndef PARTY
#define PARTY

#include <string>
using std::string;
#include <vector>
using std::vector;

#include "common.hh"

class member
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
		member (sex_ its_sex, string its_name);
};

class party
{
	vector<member> members;
	int food; // in LB
	int ammo; // in shots
	int oxen;
	int money;

	public:
	void init_party ();
	void shop ();
	party();
};
#endif 
