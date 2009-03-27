#include <memory>

#include <cpptest.h>

#include "party.hh"

class TestParty : public Test::Suite
{
    void TestConstructor ();
    void TestAddMember ();
    void TestActivity ();
    public:
    TestParty()
    {
        TEST_ADD(TestParty::TestConstructor);
        TEST_ADD(TestParty::TestAddMember);
        TEST_ADD(TestParty::TestActivity);
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
    std::auto_ptr<MemberContainer> actives;

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

