#include <cassert>

#include <glibmm.h>

#include "party.hh"
#include "runners.hh"
#include "world.hh"

namespace Freetrail{
    namespace Runner
{

Status::Status () :
    _state (KEEP_RUNNING)
{}

///In case we're not moving sequentially, this will jump us somewhere.
/**
 *@param track name of the Track to jump to.
 */
void Status::setNextTrack (const Glib::ustring& track)
{
    _state = KEEP_RUNNING;
    _next_track = track;
}

///When we've won or lost, this needs to be called.
void Status::setStatus (const state new_state)
{
    _state = new_state;
}

///@param party the party we're moving.
///@param map the map we're moving the party through.
IMapRunner::IMapRunner (Party& party, const Map& map) :
    Runner::Runner(party), _map(map)
{
}

Status IMapRunner::run()
{
    Status stat;
    stat.setNextTrack (_map.getStartTrack ());

    while (stat.KeepRunning()) // Default value
    {
        const Track& _curr_track = _map.find (stat.getNextTrack ());
        ITrackRunner tRun (_party, _curr_track);
        stat = tRun.run ();
    }

    return stat;
}

///@param party: the party we're moving.
///@param track: the track we're moving the party through.
ITrackRunner::ITrackRunner (Party& party, const Track& track) :
    Runner(party), _track(track)
{
}

///Runs a party through all the Locations in the Track.
///@relates: Track
Status ITrackRunner::run()
{
    Status stat;
    for (unsigned int i = 0; i < _track.size () && stat.KeepRunning(); i++)
    {
        ILocationRunner lRun (_party, _track[i]);
        stat = lRun.run ();
    }

    return stat;
}

///@param party: the party we're moving
///@param location: the location we're at, where stuff is happening.
ILocationRunner::ILocationRunner (Party& party, const LocationPtr location) :
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
