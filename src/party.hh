#ifndef PARTY
#define PARTY

#include <vector>

#include <glibmm.h>

#include "common.hh"

enum sex {MALE, FEMALE};

/// How hungry is someone, are they alive? What sicknesses, etc.
class Health
{
    unsigned int _hunger; ///< How hungry are we, on a scale of 1 -- 7?
    bool _is_alive; ///< Are we quick or dead?

    public:
    Health () : _hunger (7), _is_alive (true) {};

    unsigned int get_hunger () const {return _hunger;};

    /// Called when a member eats.
    unsigned int feed ();
    /// Called when a member can't eat.
    unsigned int starve ();
};

class Member
{
    private:
        sex _sex;
        Health _health;
        int _hunting_skill;
        Glib::ustring _name;

    public:
        Member (const sex its_sex, const Glib::ustring& its_name);
        bool operator == (const Member& rhs) const {return _name == rhs._name;};

        /// Called when a member eats.
        unsigned int feed () {return _health.feed ();};
        unsigned int starve ();

        sex get_sex () const;
        unsigned int get_hunger () const {return _health.get_hunger ();};
        int get_hunting_skill () const;
        const Glib::ustring& get_name () const;

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
    Party (const MemberContainer& members);
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
};
#endif 
