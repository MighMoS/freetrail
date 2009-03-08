#include <cassert>

#include "runners.hh"
#include "world.hh"

namespace Freetrail{
    namespace Runner
{

Status::Status () :
    _next_track(NULL), _state (KEEP_RUNNING)
{}

///In case we're not moving sequentially, this will jump us somewhere.
void Status::setNextTrack (const Track* track)
{
    _next_track = track;
}

///When we've won or lost, this needs to be called.
void Status::setStatus (const state state)
{
    _state = state;
}

///Abstract constructor.
Runner::Runner (Party* party) : _party(party)
{
    assert (party != NULL);
}

///@param party: the party we're moving.
///@param map: the map we're moving the party through.
IMapRunner::IMapRunner (Party* party, const Map* map) :
    Runner::Runner(party), _map(map),
    _curr_track(map->getStartTrack())
{
    assert (map != NULL);
    assert (_curr_track != NULL);
}

Status IMapRunner::run()
{
    Status stat;
    while (stat.KeepRunning()) // Default value
    {
        ITrackRunner tRun (_party, _curr_track);
        tRun.run ();
        _curr_track = stat.getNextTrack ();
    }

    return stat;
}

///@param party: the party we're moving.
///@param track: the track we're moving the party through.
ITrackRunner::ITrackRunner (Party* party, const Track* track) :
    Runner(party), _track(track),
    _curr_location(track->get_stop(0))
{
    assert (_track != NULL);
}

///Runs a party through all the Locations in the Track.
///@relates: Track
Status ITrackRunner::run()
{
    Status stat;
    while (stat.KeepRunning() == true)
    {
        ILocationRunner lRun (_party, _curr_location);
        lRun.run ();
        if (stat.getNextTrack () != NULL)
        {
            return stat;
        }
    }

    return stat;
}

///@param party: the party we're moving
///@param location: the location we're at, where stuff is happening.
ILocationRunner::ILocationRunner (Party* party, const Location* location) :
    Runner(party), _location(location)
{
    assert (_location != NULL);
}

///Doesn't really do anything but call the Location's run function.
Status ILocationRunner::run()
{
    Status stat;

    stat = _location->run (_party);

    return stat;
}

} // namespace Runner
} // namespace Freetrail
