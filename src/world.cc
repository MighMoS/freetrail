#include <algorithm>
#include <cassert>
#include <iostream>
#include <sstream>

#include <glibmm.h>
#include <libxml++/libxml++.h>

#include "world.hh"

/**
 * Extracts the name tag from an element.
 * @param[in,out] iter an Iterator to extract name from. Upon completion,
     it will lack the name tag.
 * @returns the name of what's being parsed.
 * @note the iterator passed in will no longer have the <name> tag.
 */
static inline Glib::ustring
extract_name (const xmlpp::Node::NodeList::const_iterator& iter)
{
    const xmlpp::Node::NodeList name_tag = (*iter)->get_children("name");
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

/**
 * Constructs an Outpost from iter
 * @returns a ready to use Outpost.
 * @relates Outpost
 */
static inline Outpost*
fill_outpost (const xmlpp::Node::NodeList::const_iterator& iter)
{
    Glib::ustring outpost_name;

    outpost_name = extract_name (iter);
    Outpost* loc = new Outpost (outpost_name);

    return loc;
};

///Creates a new location based off the XML pointed to by stop_iter
///@relates Path
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

///@relates: ForkOption
static inline ForkOptionPtr
fill_jump (const xmlpp::Node::NodeList::const_iterator& iter)
{
    Glib::ustring destination;
    Glib::ustring description;
    const xmlpp::Element* jump = dynamic_cast<const xmlpp::Element*> (*iter);
    const xmlpp::TextNode* description_text = jump->get_child_text ();
    xmlpp::Attribute* jump_dest = jump->get_attribute(Glib::ustring("dest"));

    destination = jump_dest->get_value ();
    description = description_text->get_content ();

    ForkOptionPtr ptr (new ForkOption(description, destination));
    return ptr;
}

///@relates Fork
/**
 *@param iter
 *@param type indicates if we are a regular Fork or a FixedJump.
 */
static inline Fork*
fill_fork (const xmlpp::Node::NodeList::const_iterator& iter,
        const Glib::ustring& type)
{
    xmlpp::Node::NodeList fork_children;
    Glib::ustring fork_name;
    ForkOptionContainer option_list;
    Fork* fork;

    fork_name = extract_name (iter);
    fork_children = (*iter)->get_children (Glib::ustring("jump"));

    // Process child nodes and text
    for (xmlpp::Node::NodeList::const_iterator
            substop_iter = fork_children.begin ();
            substop_iter != fork_children.end (); substop_iter++)
    {
        const xmlpp::TextNode* nodeText =
            dynamic_cast<const xmlpp::TextNode*>(*substop_iter);
        if (nodeText && nodeText->is_white_space()) continue;

        ForkOptionPtr opt (fill_jump (substop_iter));
        option_list.push_back(opt);
    }

    if (type == "userjump")
        fork = new Fork (fork_name, option_list);
    else if (type == "fixedjump")
        fork = new FixedJump (fork_name, option_list);
    else
        assert (0 && "Jump was not a user jump or fixed jump");

    return fork;
}

///Fills out a generic Location by passing types to handlers.
///@relates: Location
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
    if (type == "userjump" || type == "fixedjump")
        return fill_fork (iter, type);

    return NULL;
}

/// Returns a complete, ready to use track
/**
 * @param track_iter iterator of who's children we will construct
 * the track from.
 * @todo use exceptions to raise errors about what we expected in the
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
        }
    }

    return new_track;
}

Map::Map (const char filename[])
{
    xmlpp::DomParser parser;
    xmlpp::Node::NodeList track_list;
    const xmlpp::Element* root_element;
    const xmlpp::Attribute* starting_track;

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
    _first_track = starting_track->get_value ();
    assert ("" != _first_track);

    track_list = rNode->get_children("track");

    // Cannot wait for the auto keyword
    // Anyway, hop through all the tracks for each of their shits
    for (xmlpp::Node::NodeList::const_iterator track_iter = track_list.begin();
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
void Track::add_location(Location* loc)
{
    assert(loc != NULL);

    // From this point forward, loc is automatically managed.
    std::tr1::shared_ptr<Location> shared_loc (loc);
    _track.push_back(shared_loc);
}

bool Track::operator == (const Glib::ustring& rhs) const
{
    return _name == rhs;
}

/**
 * Returns the nth location (0 based).
 * @note caller does not have to @c delete the returned Location*
 */
const Location* Track::get_stop(const unsigned int pos) const
{
    assert (pos <= _track.size());
    return _track[pos].get();
}

unsigned int Track::size() const
{
    return _track.size();
}

void Map::add_track(const Track& track)
{
    _all_tracks.insert (track);
}

const Track* Map::getStartTrack () const
{
    return find (_first_track);
}

/**
 * @param[in] track_name the name of the Track to retrieve.
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

