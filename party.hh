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

	public:
	void init_party ();
};
#endif 
