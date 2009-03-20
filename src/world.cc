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
 *@todo Once we've got everything, make sure that we can go anywhere we need to
 *@todo Try to load from ./maps and XDG_DATADIR/maps
 */
Map::Map (const Glib::ustring& first_track, TrackContainer& all_tracks) :
    _all_tracks(all_tracks), _first_track (first_track)
{
}

Track::Track(const Glib::ustring& name) : _name(name) {};

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
const LocationPtr Track::get_stop(const unsigned int pos) const
{
    assert (pos <= _track.size());
    return _track[pos];
}

unsigned int Track::size() const
{
    return _track.size();
}

void Map::add_track(const Track& track)
{
    _all_tracks.insert (track);
}

const Glib::ustring& Map::getStartTrack () const
{
    return _first_track;
}

/**
 * @param track_name the name of the Track to retrieve.
 * @note The caller should NOT @c delete the returned Track.
 */
const Track* Map::find (const Glib::ustring& track_name) const
{
    // Convert the iterator to a regular pointer
    const Track* found = &*(_all_tracks.find (track_name));
    assert (found != &*(_all_tracks.end ()));
    return found;
}

unsigned int Map::size () const
{
    return _all_tracks.size();
}

