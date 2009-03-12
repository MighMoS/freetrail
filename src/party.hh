#ifndef PARTY
#define PARTY

#include <vector>

#include <glibmm.h>

#include "common.hh"

enum sex {MALE, FEMALE};
enum health {DEAD, DYING, POOR, FAIR, GOOD, EXCELLENT};

class Member
{
    private:
        sex _sex;
        health _health;
        int _hunting_skill;
        Glib::ustring _name;

    public:
        sex get_sex () const;
        health get_health () const;
        int get_hunting_skill () const;
        const Glib::ustring& get_name () const;
        Member (const sex its_sex, const Glib::ustring& its_name);
};

typedef std::vector<Member> MemberContainer;
/// The stars of the show.
class Party
{
    MemberContainer _members;
    int _food; ///<Amount of food the Party has, in LBs
    unsigned int _ammo; ///<Amount of amunition the Party has, in rounds.
    unsigned int _oxen;
    unsigned int _money;
    unsigned int _speed;

    public:
    const MemberContainer* get_members() const;

    unsigned int get_speed () const;
    unsigned int get_distance () const;
    unsigned int get_ammo () const;
    int get_food () const;
    unsigned int get_money () const;
    unsigned int get_oxen () const;
    int eat_food ();
    unsigned int buy_ammo(const unsigned int amount);
    unsigned int buy_food(const unsigned int amount);
    unsigned int buy_oxen(const unsigned int amount);
    void init_party ();

    Party();
};
#endif 
