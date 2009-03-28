#include <memory>

#include <cpptest.h>

#include "location.hh"
#include "world.hh"

class TestTrack : public Test::Suite
{
    void TestConstructor();

    public:
    TestTrack ()
    {
        TEST_ADD(TestTrack::TestConstructor);
    }
};

void TestTrack::TestConstructor ()
{
    Track sample_track ("test");

    TEST_ASSERT (sample_track.size() == 0);
    TEST_ASSERT (sample_track == "test");
}

class TestMap : public Test::Suite
{
    void TestConstructor();

    public:
    TestMap ()
    {
        TEST_ADD(TestMap::TestConstructor);
    }
};

void TestMap::TestConstructor ()
{
    TrackContainer all_tracks;
    all_tracks.insert (Track("not-first"));
    all_tracks.insert (Track("first"));

    TEST_ASSERT (all_tracks.size() == 2);

    Map sample_map ("first", all_tracks);
    TEST_ASSERT (sample_map.size() == 2);
    TEST_ASSERT (sample_map.getStartTrack() == "first");
}

int main ()
{
    Test::Suite ts;
    ts.add (std::auto_ptr<Test::Suite>(new TestTrack));
    ts.add (std::auto_ptr<Test::Suite>(new TestMap));

    Test::TextOutput output (Test::TextOutput::Verbose);
    bool it_worked = ts.run (output);

    if (it_worked)
        return 0;
    return 1;
}
