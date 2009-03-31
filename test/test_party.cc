#include <memory>

#include <cpptest.h>

#include "party.hh"

class TestParty : public Test::Suite
{
    void TestConstructor ();
    void TestAddMember ();
    void TestActivity ();
    void TestEatFood ();

    public:
    TestParty()
    {
        TEST_ADD(TestParty::TestConstructor);
        TEST_ADD(TestParty::TestAddMember);
        TEST_ADD(TestParty::TestActivity);
        TEST_ADD(TestParty::TestEatFood);
    };
};

void TestParty::TestConstructor ()
{
    Party some_party;

    TEST_ASSERT (some_party.size () == 0);
}

void TestParty::TestAddMember ()
{
    Party some_party;

    some_party.add_member (Member("John", MALE));
    some_party.add_member (Member("Mary", FEMALE));
    some_party.add_member (Member("Paul", MALE));

    TEST_ASSERT (some_party.size () == 3);

    some_party.add_member (Member("Mary", FEMALE));
    some_party.add_member (Member("Paul", MALE));

    TEST_ASSERT (some_party.size () == 3);
}

void TestParty::TestActivity ()
{
    Party some_party;
    MemberContainerPtr actives;

    some_party.add_member (Member("John", MALE));
    some_party.add_member (Member("Mary", FEMALE));
    some_party.add_member (Member("Paul", MALE));

    TEST_ASSERT (some_party.size () == 3);

    actives = some_party.get_active_members ();
    TEST_ASSERT (actives->size () == 3);

    actives = some_party.get_inactive_members ();
    TEST_ASSERT (actives->size () == 0);

    Member kenny ("Kenny", MALE);
    for (unsigned int i = 0; i < 7; i++) kenny.starve ();
    some_party.add_member (kenny);

    TEST_ASSERT (some_party.size () == 3);
    actives = some_party.get_active_members ();
    TEST_ASSERT (actives->size () == 3);
    actives = some_party.get_inactive_members ();
    TEST_ASSERT (actives->size () == 1);
}

///todo This test is horribly fragile.
void TestParty::TestEatFood ()
{
    Party some_party;
    MemberContainerPtr actives;
    TEST_ASSERT (some_party.get_food () == 100);

    Member John ("John", MALE);
    Member Mary ("Mary", FEMALE);
    Mary.starve ();
    Member Steve ("Steve", MALE);
    Steve.starve ();
    Steve.starve ();
    Member Ann ("Ann", FEMALE);
    Ann.starve ();
    Ann.starve ();
    Ann.starve ();

    // John should be at 7, Mary at 6, and Steve at 5, and Ann at 4.

    some_party.add_member (John);
    some_party.add_member (Mary);
    some_party.add_member (Steve);
    some_party.add_member (Ann);
    TEST_ASSERT (some_party.size () == 4);

    // (4 members * 5lbs / day) * 5 = 100 (default food)
    for (unsigned int i = 0; i < 5; i++) some_party.eat_food ();
    TEST_ASSERT (some_party.size () == 4);
    TEST_ASSERT (some_party.get_food () == 0);
    for (unsigned int i = 0; i < 5; i++) some_party.eat_food ();
    TEST_ASSERT (some_party.size () == 4);
    TEST_ASSERT (some_party.get_food () == 0);
    // Member should be throughly starved now, ironically, except Ann
    // As she got the first bits of food to nurse her back to health.
    some_party.eat_food ();
    TEST_ASSERT (some_party.size () == 1);
    some_party.eat_food ();
    TEST_ASSERT (some_party.size () == 0);
}

int main ()
{
    Test::Suite ts;
    ts.add(std::auto_ptr<Test::Suite>(new TestParty));

    Test::TextOutput output (Test::TextOutput::Verbose);
    bool it_worked = ts.run (output);

    if (it_worked)
        return 0;
    return 1;
}

