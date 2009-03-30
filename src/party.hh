#ifndef PARTY
#define PARTY

#include <memory>
#include <set>

#include <glibmm.h>

#include "common.hh"

enum sex {MALE, FEMALE};

/// How hungry is someone, are they alive? What sicknesses, etc.
class Health
{
    unsigned int _hunger; ///< How hungry are we, on a scale of 1 -- 7?
    bool _is_alive; ///< Once killed, a party member can not do anything.

    public:
    Health () : _hunger (7), _is_alive (true) {};

    /// A number 1 -- 7, detailing how hungry the member is.
    unsigned int get_hunger () const {return _hunger;};

    /// Are we quick or dead?
    bool is_alive () const {return _is_alive;};

    /// Called when a member eats.
    unsigned int feed ();
    /// Called when a member can't eat.
    unsigned int starve ();
};

/// A person object, and everything about them.
class Member
{
    private:
        const Glib::ustring _name; ///<Name. Should be unique.
        const sex _sex; ///<Male or female
        Health _health; ///<Hunger, sickness, all contained here.
        //int _hunting_skill;

    public:
        Member (const Glib::ustring& its_name, const sex its_sex);
        /// Arbitrary sorting by hunger and name.
        friend bool operator < (const Member& lhs, const Member& rhs);
        /// Two party members are the same if they have the same name.
        friend bool operator == (const Member& lhs, const Member& rhs)
        {
            return lhs._name == rhs._name;
        }

        /// Is this member still alive and breathing?
        bool is_alive () const {return _health.is_alive ();};
        /// Called when a member eats.
        unsigned int feed () {return _health.feed ();};
        /// Called when a member can't eat.
        unsigned int starve ();

        /// Male or female?
        sex get_sex () const;
        /// How hungry (close to death) is someone?
        unsigned int get_hunger () const {return _health.get_hunger ();};
        //int get_hunting_skill () const {return _hunting_skill;};
        const Glib::ustring& get_name () const;

};

typedef std::set<Member> MemberContainer;
typedef std::auto_ptr<MemberContainer> MemberContainerPtr;
/// The stars of the show.
class Party
{
    MemberContainer _members; ///<Who's with us.
    int _food; ///<Amount of food the Party has, in LBs
    unsigned int _ammo; ///<Amount of amunition the Party has, in rounds.
    unsigned int _oxen; ///<How many beasts of burden are pulling our stuff?
    unsigned int _money; ///<How much money we have to spend.
    unsigned int _speed; ///<How fast we can go, after taking weight pulled into account.

    public:
    /// Construct a new party with no members and default parameters.
    Party ();
    /// Construct a new party with the given members and default parameters.
    Party (const MemberContainer& members);
    /// Get everyone who's traveling with us.
    const MemberContainer* get_members () const;
    /// Get everyone who's not dead.
    MemberContainerPtr get_active_members () const;
    /// Get everyone who's dead.
    MemberContainerPtr get_inactive_members () const;

    /// Add someone to the party.
    void add_member (const Member& member);
    /// Returns the number of members who aren't dead yet.
    unsigned int size () const;
    unsigned int get_speed () const;
    unsigned int get_ammo () const;
    int get_food () const;
    unsigned int get_money () const;
    unsigned int get_oxen () const;
    /// Consume food, and face the consequences if we can't.
    unsigned int eat_food ();
    unsigned int buy_ammo(const unsigned int amount);
    unsigned int buy_food(const unsigned int amount);
    unsigned int buy_oxen(const unsigned int amount);
};
#endif 
