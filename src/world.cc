#include <cassert>
#include <iostream>
#include <string>
using std::string;
#include <sstream>

#include <libxml++/libxml++.h>

#include "world.hh"

///@relates Track
///Returns the track identification number pointed to by track_iter
static inline unsigned int
get_track_number(xmlpp::Node::NodeList::const_iterator track_iter)
{
    std::stringstream ss;
    unsigned int track_no;
    const xmlpp::Element* nodeElement = dynamic_cast<xmlpp::Element*>(*track_iter);
    const xmlpp::Element::AttributeList& attributes =
        nodeElement->get_attributes();

    for(xmlpp::Element::AttributeList::const_iterator attr_iter =
            attributes.begin(); attr_iter != attributes.end(); attr_iter++)
    {
        const xmlpp::Attribute* attribute = *attr_iter;
        ss << attribute->get_value();
        ss >> track_no;
        break; // Don't keep looking for another attribute
    }

    return track_no;
}

#if 0
// XXX Fix copy and paste
static std::vector<unsigned int>*
fill_switch (const xmlpp::Node::NodeList list)
{
    for (xmlpp::Node::NodeList::const_iterator switch_iter = list.begin();
            switch_iter != list.end(); switch_iter++)
    {
        const xmlpp::Element* nodeElement =
            dynamic_cast<const xmlpp::Element*>(*substop_iter);
        const Glib::ustring name = (*switch_iter)->get_name();
        const xmlpp::TextNode* fNode = nodeElement->get_child_text();

        if (name == "choice")
        {
            stop_name = fNode->get_content();
            assert ("" != stop_name);
            continue;
        }
    }
}
#endif

///@relates: location
///Creates a new location based off the XML pointed to by stop_iter
static inline
Path* fill_stop(xmlpp::Node::NodeList::const_iterator stop_iter)
{
    xmlpp::Element* nodeElement = dynamic_cast<xmlpp::Element*>(*stop_iter);
    const xmlpp::Element::AttributeList& attributes =
        nodeElement->get_attributes();
    Glib::ustring stop_name;
    unsigned int stop_length;
    bool stop_outpost = false, stop_can_hunt = false;

    for(xmlpp::Element::AttributeList::const_iterator attr_iter =
            attributes.begin(); attr_iter != attributes.end(); attr_iter++)
    {
        const xmlpp::Attribute* attribute = *attr_iter;
        const Glib::ustring attribute_name = attribute->get_name();
        const Glib::ustring attribute_value = attribute->get_value();

        // Stop if we don't actually have anything
        if (attribute_name.empty()) continue;
        if (attribute_name == "outpost")
        {
            if (attribute_value != "0")
                stop_outpost = true;
        }
        if (attribute_name == "hunting")
        {
            if (attribute_value != "0")
                stop_can_hunt = true;
        }
    }

    xmlpp::Node::NodeList list = (*stop_iter)->get_children();

    // Process child nodes
    for (xmlpp::Node::NodeList::const_iterator substop_iter = list.begin();
            substop_iter != list.end(); substop_iter++)
    {
        const xmlpp::TextNode* nodeText =
            dynamic_cast<const xmlpp::TextNode*>(*substop_iter);

        if (nodeText && nodeText->is_white_space()) continue;
        const Glib::ustring name = (*substop_iter)->get_name();
        const xmlpp::Element* nodeElement =
            dynamic_cast<const xmlpp::Element*>(*substop_iter);
        const xmlpp::TextNode* fNode = nodeElement->get_child_text();

        if (name == "name")
        {
            stop_name = fNode->get_content();
            assert ("" != stop_name);
            continue;
        }
        if (name == "length")
        {
            std::stringstream ss;
            ss << fNode->get_content();
            ss >> stop_length;
            continue;
        }
        if (name == "switch")
        {
            const xmlpp::Node::NodeList choices_list =
                (*substop_iter)->get_children("choice");
            //fork = fill_switch (choice_list);
        }
    }

    Path* loc = new Path(stop_name, stop_length,
            stop_outpost, stop_can_hunt);
    if (loc == NULL)
    {
        std::cerr << "Out of memory!\n";
        exit (1);
    }
    return loc;
}

static inline Map* parse_locations(const char filename[] = "map.xml")
{
    // In the future this should not be const
    string location_name, buffer;
    Map* map = new Map();
    if (map == NULL)
    {
        std::cerr << "Out of memory!\n";
        exit (1);
    }

    // No align because things things should succeed
    // The syntax is only for failure
    try
    {
    xmlpp::DomParser parser;
    parser.parse_file(filename);
    parser.set_validate();
    parser.set_substitute_entities();

    if (parser)
    {
    // Parse the document
    const xmlpp::Node* rNode = parser.get_document()->get_root_node();
    if (rNode->get_name() != "freetrail")
    {
        std::cerr << "Expected root node to be \"freetrail\"!\n";
        exit(2);
    }

    xmlpp::Node::NodeList list = rNode->get_children("track");

    // Cannot wait for the auto keyword
    // Anyway, hop through all the tracks for each of their shits
    for (xmlpp::Node::NodeList::const_iterator track_iter = list.begin();
            track_iter != list.end(); track_iter++)
    {
        Track curr_track(get_track_number(track_iter));
        const xmlpp::Node::NodeList stop_list =
            (*track_iter)->get_children("stop");
        for(xmlpp::Node::NodeList::const_iterator stop_iter = stop_list.begin();
                stop_iter != stop_list.end(); stop_iter++)
        {
            std::cerr << "Filling location...\n\t";
            Path* loc = fill_stop(stop_iter);
            assert (loc != NULL);
            curr_track.add_location(loc);
            std::cerr << "Filled location " << loc->get_name() << std::endl;
            delete loc;
        }
        map->add_track(curr_track);
    } // for (auto track_iter = list.begin)
    } // if(parser)
    } // try
    catch(const std::exception& ex)
    {
        std::cerr << "Exception caught: " << ex.what() << std::endl;
        exit (1);
    }

    return map;
}

Track::Track(const int number) : track_number(number) {};

/**
 * Add a location to this track.
 * @param[in] loc   Initialized pointer to location which will be added to this track.
 *
 * Side effects include the previous added track will be updated when a new one is inserted.
 * @bug: this is not thread safe
 * @bug: this sucks.
 */
void Track::add_location(location* loc)
{
    static Path* last_path = NULL;
    Path* curr_path;

    assert(loc != NULL);

    // Update old path
    if (last_path != NULL)
        last_path->set_next_location(loc);

    // Set old path to current path
    curr_path = dynamic_cast<Path*>(loc);
    if (curr_path)
        last_path = curr_path;
    else
        last_path = NULL;
    track.push_back(loc);
}

/**
 * Returns the nth location (0 based).
 * @notes caller does not have to delete the returned location*
 */
const location* Track::get_stop(const unsigned int pos) const
{
    assert (pos <= track.size());
    return track[pos];
}

unsigned int Track::size() const
{
    return track.size();
}

void Map::add_track(const Track& track)
{
    all_tracks.push_back(track);
}

const Track* Map::getStartTrack () const
{
    return (const Track*) 0xdeadbeef;
}

const Track* Map::get_track(const unsigned int pos) const
{
    return &all_tracks[pos];
}

unsigned int Map::size () const
{
    return all_tracks.size();
}

World::World()
{
    map = parse_locations();
}

World::~World()
{
    delete map;
}

const location* World::get_curr_loc(const unsigned int track,
                                    const unsigned int pos) const
{
#ifdef DEBUG
    std::cout << "Get current location: " << track << ", " << pos << std::endl;
#endif
    return map->get_track(track)->get_stop(pos);
}

const Map* World::get_map () const
{
    return map;
}

