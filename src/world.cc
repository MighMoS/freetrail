#include <algorithm>
#include <cassert>
#include <sstream>
#include <string>

#include <glibmm.h>
#include <libxml++/libxml++.h>

#include "common.hh"
#include "parser.hh"
#include "world.hh"

/**
 *@param first_track Where the user will start his/her journey.
 *@param all_tracks Container to everything this map should hold.
 */
Map::Map (const Glib::ustring& first_track, const TrackContainer& all_tracks)
    : _all_tracks(all_tracks), _first_track (first_track)
{
}

/**
 *@param name the name this track will be referenced by
 */
Track::Track (const Glib::ustring& name) : _name(name) {};

/**
 * Add a Location to this Track.
 * @param[in] loc   Initialized pointer to location which will be added to this track.
 * @note The caller should NOT @c delete the pointer passed in.
 */
void Track::add_location(const LocationPtr loc)
{
    assert(loc != NULL);

    _track.push_back(loc);
}

bool Track::operator == (const Glib::ustring& rhs) const
{
    return _name == rhs;
}

/**
 * @param pos 0 based integer to retrieve the nth Location.
 * @note caller does not have to @c delete the returned Location*
 */
const LocationPtr Track::operator[](const unsigned int pos) const
{
    assert (pos <= _track.size());
    return _track[pos];
}

unsigned int Track::size() const
{
    return _track.size();
}

const Glib::ustring& Map::getStartTrack () const
{
    return _first_track;
}

/**
 * @param track_name the name of the Track to retrieve.
 * @note The caller should NOT @c delete the returned Track.
 */
const Track& Map::find (const Glib::ustring& track_name) const
{
    // Convert the iterator to a regular pointer
    const Track* found = &*(_all_tracks.find (track_name));
    assert (found != &*(_all_tracks.end ()));
    return *found;
}

unsigned int Map::size () const
{
    return _all_tracks.size();
}

