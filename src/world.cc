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
Map::Map (const std::string& filename)
{
    xmlpp::DomParser parser;
    xmlpp::Node::NodeList track_list;
    const xmlpp::Element* root_element;
    const xmlpp::Attribute* starting_track;
    const std::string file_path (FREETRAIL_MAPSDIR); // Defined in configure.ac

    std::string complete_file_name = Glib::build_filename (file_path, filename);
    Freetrail::Debug ("Loading file: " + complete_file_name);

    try
    {
        parser.parse_file (complete_file_name);
        parser.set_validate ();
        parser.set_substitute_entities ();
    }
    // Swallow any error libxml++ gives us and rethrow it.
    catch (const std::exception& ex)
    {
        MapParsingException e (std::string("error parsing XML:\n\t") + ex.what ());
        throw e;
    }

    // Parse the document
    const xmlpp::Node* rNode = parser.get_document()->get_root_node();
    if (rNode->get_name() != "freetrail")
        throw MapParsingException ("expected root node to be \"freetrail\"!");
    
    root_element = dynamic_cast<const xmlpp::Element*>(rNode);
    starting_track = root_element->get_attribute (Glib::ustring("start"));
    _first_track = starting_track->get_value ();
    assert ("" != _first_track);

    track_list = rNode->get_children("track");

    // Cannot wait for the auto keyword
    // Anyway, hop through all the tracks for each of their shits
    for (xmlMapIter track_iter = track_list.begin();
            track_iter != track_list.end(); track_iter++)
    {
        Track* curr_track;
        curr_track = fill_track (track_iter);
        assert (curr_track != NULL);
        add_track(*curr_track);
        delete (curr_track);
    }

    // Really there does need to be some kind of sanity check here.
#ifndef NDEBUG
    Freetrail::Debug ("Found tracks:");
    for (TrackContainer::iterator i = _all_tracks.begin (); i != _all_tracks.end (); i++)
    {
        Freetrail::Debug ("Sanity check:");
        Freetrail::Debug ("\tAdded " + i->get_name ());
        Freetrail::Debug ("\tCan reference " +
                (_all_tracks.find (i->get_name ()))->get_name());
        assert (_all_tracks.find (i->get_name ()) != _all_tracks.end ());
    }
#endif
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
    return &*_all_tracks.find (track_name);
}

unsigned int Map::size () const
{
    return _all_tracks.size();
}

