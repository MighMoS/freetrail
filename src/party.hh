#ifndef PARTY
#define PARTY

#include <string>
#include <vector>

#include "common.hh"

class Member
{
    private:
        sex_ sex;
        health_ health;
        int hunting_skill;
        std::string name;

    public:
        sex_ get_sex () const;
        health_ get_health () const;
        int get_hunting_skill () const;
        std::string get_name () const;
        Member (const sex_ its_sex, const std::string& its_name);
};

/// The stars of the show.
class Party
{
    std::vector<Member> members;
    int food; // in LB
    int ammo; // in shots
    int oxen;
    int money;
    int speed;

    public:
    const std::vector<Member>* get_members() const;

    int get_speed () const;
    int get_distance () const;
    unsigned int get_ammo () const;
    unsigned int get_food () const;
    unsigned int get_money () const;
    unsigned int get_oxen () const;
    unsigned int eat_food ();
    unsigned int buy_ammo(const unsigned int amount);
    unsigned int buy_food(const unsigned int amount);
    unsigned int buy_oxen(const unsigned int amount);
    void init_party ();

    Party();
};
#endif 
