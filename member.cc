#include "member.hh"

member::member (sex_ its_sex, string its_name)
{
        sex = its_sex;
        name = its_name;
}

string member::get_name () const
{
	return name;
}
