#include <algorithm>
#include <cassert>
#include <iostream>
#include <string>
using std::string;
#include <sstream>

#include <libxml++/libxml++.h>

#include "world.hh"

/**
 * Extracts the name tag from an element.
 * @returns the name of what's being parsed.
 * @note the iterator passed in will no longer have the <name> tag.
 */
static inline Glib::ustring
extract_name (const xmlpp::Node::NodeList::const_iterator& iter)
{
    const xmlpp::Node::NodeList name_tag = (*iter)->get_children("name");
    if (name_tag.size () == 0)
        return Glib::ustring("");
    const xmlpp::Element* nodeElement =
        dynamic_cast<const xmlpp::Element*> (*(name_tag.begin()));
    const xmlpp::TextNode* node_text = nodeElement->get_child_text ();

    xmlpp::Node::NodeList node_list = (*iter)->get_children ();
    Glib::ustring name;

    name = node_text->get_content ();

    // replace these with an exceptions
    //assert (name_tag.size () == 1);
    assert (name != "");

    // Remove the name tag from our list
    std::remove(node_list.begin(), node_list.end(), *(name_tag.begin()));

    return name;
}

static inline Outpost*
fill_outpost (const xmlpp::Node::NodeList::const_iterator& iter)
{
    Glib::ustring outpost_name;

    outpost_name = extract_name (iter);
    Outpost* loc = new Outpost (outpost_name);

    return loc;
};

///@relates: path
///Creates a new location based off the XML pointed to by stop_iter
/**
 * <path><name>thisPathName</name><length>100</path>
 * would create a path with a length of 100 called "thisPathName"
 */
static inline
Path* fill_path (const xmlpp::Node::NodeList::const_iterator& stop_iter)
{
    xmlpp::Node::NodeList path_children;
    Glib::ustring path_name;
    unsigned int path_length;

    path_name = extract_name (stop_iter);
    path_children = (*stop_iter)->get_children ();

    // Process child nodes and text
    for (xmlpp::Node::NodeList::const_iterator
            substop_iter = path_children.begin ();
            substop_iter != path_children.end (); substop_iter++)
    {
        const xmlpp::TextNode* nodeText =
            dynamic_cast<const xmlpp::TextNode*>(*substop_iter);

        if (nodeText && nodeText->is_white_space()) continue;

        const Glib::ustring element_name = (*substop_iter)->get_name ();
        const xmlpp::Element* nodeElement =
            dynamic_cast<const xmlpp::Element*>(*substop_iter);
        const xmlpp::TextNode* fNode = nodeElement->get_child_text ();

        if (element_name == "length")
        {
            std::stringstream ss;
            ss << fNode->get_content();
            ss >> path_length;
            assert (0 < path_length);
            continue;
        }
    }

    Path* loc = new Path (path_name, path_length);
    return loc;
}

static inline Location*
fill_location (const xmlpp::Node::NodeList::const_iterator& iter)
{
    Glib::ustring type;

    type = (*iter)->get_name ();

#ifndef NDEBUG
    std::cerr << "Currently processing an element of type " <<
        type << std::endl;
#endif
    if (type == "path")
        return fill_path (iter);
    if (type == "outpost")
        return fill_outpost (iter);
    if (type == "userjump"){}
    if (type == "fixedjump"){}

    return NULL;
}

/// Returns a complete, ready to use track
/**
 * @param[in] track_iter iterator of who's children we will construct
 * the track from.
 * @todo: use exceptions to raise errors about what we expected in the
 * document
 */
static inline
Track* fill_track (const xmlpp::Node::NodeList::const_iterator& track_iter)
{
    xmlpp::Node::NodeList track_list = (*track_iter)->get_children ();
    Track* new_track;
    Glib::ustring name;

    name = extract_name (track_iter);
    new_track = new Track (name);
#ifndef NDEBUG
    std::cerr << "Created track " << name << std::endl;
#endif

    // Get all  our children
    for(xmlpp::Node::NodeList::const_iterator
            track_iter = track_list.begin();
            track_iter != track_list.end(); track_iter++)
    {
        if ((*track_iter)->get_name () == "text") continue;
        Location* loc = fill_location (track_iter);
        if (loc != NULL)
        {
            new_track->add_location (loc);
#ifndef NDEBUG
            std::cerr << "Filled location " << loc->get_name() << std::endl;
#endif
            delete loc;
        }
    }

    return new_track;
}

Map::Map (const char filename[])
{
    xmlpp::DomParser parser;
    const xmlpp::Element* root_element;
    xmlpp::Node::NodeList track_list;
    const xmlpp::Attribute* starting_track;
    Glib::ustring starting_track_name;

    try
    {
        parser.parse_file(filename);
        parser.set_validate();
        parser.set_substitute_entities();
    }
    catch (const std::exception& ex)
    {
        std::cerr << "Exception caught validating the map: "
            << ex.what() << std::endl;
        exit (1);
    }

    // Parse the document
    const xmlpp::Node* rNode = parser.get_document()->get_root_node();
    if (rNode->get_name() != "freetrail")
    {
        std::cerr << "Expected root node to be \"freetrail\"!\n";
        exit(2);
    }
    
    root_element = dynamic_cast<const xmlpp::Element*>(rNode);
    starting_track = root_element->get_attribute (Glib::ustring("start"));
    starting_track_name = starting_track->get_value ();
    assert ("" != starting_track_name);

    track_list = rNode->get_children("track");

    bool first_filled = false;
    // Cannot wait for the auto keyword
    // Anyway, hop through all the tracks for each of their shits
    for (xmlpp::Node::NodeList::const_iterator track_iter = track_list.begin();
            track_iter != track_list.end(); track_iter++)
    {
        Track* curr_track;
        curr_track = fill_track (track_iter);
        assert (curr_track != NULL);
        if (first_filled == false && *curr_track == starting_track_name)
        {
            first_filled = true;
            _firstTrack = curr_track;
        }
        this->add_track(*curr_track);
        delete curr_track;
    }

    assert (first_filled != false);
}

Track::Track(const Glib::ustring& name) : _name(name) {};

Track::~Track ()
{
}

/**
 * Add a location to this track.
 * @param[in] loc   Initialized pointer to location which will be added to this track.
 */
void Track::add_location(Location* loc)
{
    assert(loc != NULL);

    _track.push_back(loc);
}

bool Track::operator == (const Glib::ustring& rhs) const
{
    return _name == rhs;
}

/**
 * Returns the nth location (0 based).
 * @notes caller does not have to delete the returned Location*
 */
const location* Track::get_stop(const unsigned int pos) const
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
    all_tracks.push_back(track);
}

const Track* Map::getStartTrack () const
{
    return _firstTrack;
}

const Track* Map::get_track(const unsigned int pos) const
{
    return &all_tracks[pos];
}

unsigned int Map::size () const
{
    return all_tracks.size();
}

