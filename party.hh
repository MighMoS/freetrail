#ifndef PARTY
#define PARTY

#include <vector>
using std::vector;
#include "member.hh"

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
