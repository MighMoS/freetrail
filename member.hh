#ifndef MEMBER
#define MEMBER

#include <string>
using std::string;

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
		member (sex_ its_sex, string its_name);
};
#endif

