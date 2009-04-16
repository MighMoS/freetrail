#include <memory>

#include <cpptest.h>

#include "location.hh"
#include "parser.hh"
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
    void TestSimpleMap();

    public:
    TestMap ()
    {
        TEST_ADD(TestMap::TestConstructor);
        TEST_ADD(TestMap::TestSimpleMap);
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

void TestMap::TestSimpleMap ()
{
    std::auto_ptr<Map> map;
    MapParser simple_map_parser ("simple_map.xml");
    map.reset (simple_map_parser.parse());

    // First track has two outposts, two paths, and a fork
    const Track& track1 = *map->find ("First");
    TEST_ASSERT (track1.size() == 5);
    TEST_ASSERT (track1.get_stop(0)->get_name() == "Fort Intro");
    TEST_ASSERT (track1.get_stop(1)->get_name() == "Wild Plains");
    TEST_ASSERT (dynamic_cast<const Path*>
            (track1.get_stop(1).get())->get_next_distance() == 150);
    TEST_ASSERT (track1.get_stop(2)->get_name() == "Swamps");
    TEST_ASSERT (dynamic_cast<const Path*>
            (track1.get_stop(2).get())->get_next_distance() == 68);
    TEST_ASSERT (track1.get_stop(3)->get_name() == "Encampment Halo");
    TEST_ASSERT (track1.get_stop(4)->get_name() == "Its a fork in the road!");
    const ForkOptionContainer& foc1 = dynamic_cast<const Fork*>
            (track1.get_stop(4).get())->get_jumps();
    TEST_ASSERT (foc1.size() == 2);
    TEST_ASSERT (foc1[0]->get_description() == "Go left");
    TEST_ASSERT (foc1[0]->get_destination() == "Second part1");
    TEST_ASSERT (foc1[1]->get_description() == "Go right");
    TEST_ASSERT (foc1[1]->get_destination() == "Second part2");

    // Second Track has one path and a fixedjump
    const Track& track2 = *map->find ("Second part1");
    TEST_ASSERT (track2.size() == 2);
    TEST_ASSERT (track2.get_stop(0)->get_name() == "Left - there's nothing here");
    TEST_ASSERT (track2.get_stop(1)->get_name() == "We're almost there");
    const ForkOptionContainer& foc2 = dynamic_cast<const FixedJump*>
            (track2.get_stop(1).get())->get_jumps();
    TEST_ASSERT (foc2.size() == 1);
    TEST_ASSERT (foc2[0]->get_description() == "You could have gotten here some other way...");
    TEST_ASSERT (foc2[0]->get_destination() == "Last Track");

    // Skip 3, its the same as 2.
    // Fourth track is just a winning path.
    const Track& track4 = *map->find ("Last Track");
    TEST_ASSERT (track4.get_stop(0)->get_name() == "You're almost there");
    TEST_ASSERT (dynamic_cast<const WinningPath*>
            (track4.get_stop(0).get())->get_next_distance() == 20);
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
