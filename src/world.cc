#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
using std::string;
#include <sstream>

#include <libxml++/libxml++.h>

#include "common.hh"
#include "world.hh"

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

static inline
location* fill_stop(xmlpp::Node::NodeList::const_iterator stop_iter)
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
        const Glib::ustring current_attribute = attribute->get_value();

        Glib::ustring attr_value (attribute->get_value());
        // Stop if we don't actually have anything
        if (attr_value.empty()) continue;
        if (current_attribute == "outpost")
        {
            if (attr_value != "0")
                stop_outpost = true;
        }
        if (current_attribute == "hunting")
        {
            if (attr_value != "0")
                stop_can_hunt = true;
        }
    }

    // TODO: Don't process subchildren, or do so more efficiently
    xmlpp::Node::NodeList list = (*stop_iter)->get_children();

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
    }

    location* loc = new location(stop_name, stop_length,
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
            location* loc = fill_stop(stop_iter);
            curr_track.add_location(*loc);
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

void Track::add_location(const location& loc)
{
    track.push_back(loc);
}

const location* Track::get_stop(const unsigned int pos) const
{
    assert (pos <= track.size());
    return &track[pos];
}

unsigned int Track::size() const
{
    return track.size();
}

void Map::add_track(const Track& track)
{
    all_tracks.push_back(track);
}

unsigned int Map::get_number_tracks () const
{
    return all_tracks.size();
}

unsigned int Map::get_track_size (unsigned int track_no) const
{
    return all_tracks[track_no].size();
}

const Track* Map::get_track(const unsigned int pos) const
{
    return &all_tracks[pos];
}

World::World()
{
    map = parse_locations();
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

location::location(const string& its_name, const unsigned int distance,
                   const bool outpost, const bool hunting) :
    name(its_name), next_distance(distance), is_outpost(outpost),
    can_hunt(hunting) {};

#ifdef DEBUG
std::ostream& operator << (std::ostream& os, const location& loc)
{
    os << "\n  Name: " << loc.name;
    os << "\n\tIs an Outpost: " << (loc.is_outpost ? "Yes" : "No");
    os << "\n\tCan Hunt: " << (loc.can_hunt ? "Yes" : "No");
    return os << "\n\tDistance: " << loc.next_distance << std::endl;
}
#endif
