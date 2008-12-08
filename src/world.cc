#include <cstdlib>
#include <iostream>
using std::cout;
using std::endl;
#include <fstream>
#include <string>
using std::string;
#include <sstream>
#include <vector>
using std::vector;

#include <libxml++/libxml++.h>

#include "common.hh"
#include "world.hh"

static int get_track_number(xmlpp::Node::NodeList::const_iterator track_iter)
{
    std::stringstream ss;
    int track_no;
    xmlpp::Element* nodeElement = dynamic_cast<xmlpp::Element*>(*track_iter);
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
}

static void parse_locations(/*vector<location>& map,*/ const char filename[] = "map.xml")
{
    // In the future this should not be const
	string location_name, buffer;
    unsigned int distance;
    bool is_outpost, can_hunt;

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
    if (rNode->get_name() != "freetrail") exit(2);

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
        }
    }
    }
    }
    catch(const std::exception& ex)
    {
        std::cerr << "Exception caught: " << ex.what() << std::endl;
        exit (1);
    }
}

Track::Track(const int number) : track_number(number) {};

World::World(const int temp, const weather conditions) : 
	temperature(temp), the_weather(conditions)
{
	parse_locations();//map);
}

int World::get_temp () const
{
	return temperature;
}

weather World::get_conditions () const
{
	return the_weather;
}

void World::set_temp (const int k)
{
	temperature = k;
}

void World::set_conditions (const weather k)
{
	the_weather = k;
}

location* World::get_curr_loc()
{
	//return &map[0];
}

location* World::get_next_loc()
{
	//return &map[1];
}

bool World::no_more() const
{
	//return map.empty();
}

location::location(const string& its_name, const unsigned int distance,
                   const bool outpost, const bool hunting) :
    name(its_name), next_distance(distance), is_outpost(outpost),
    can_hunt(hunting) {};

std::ostream& operator << (std::ostream& os, const location& loc)
{
	os << "\n  Name: " << loc.name;
	os << "\n\tIs an Outpost: " << (loc.is_outpost ? "Yes" : "No");
	os << "\n\tCan Hunt: " << (loc.can_hunt ? "Yes" : "No");
	return os << "\n\tDistance: " << loc.next_distance << endl;
}

void World::pop_curr_loc()
{
	//map.erase(map.begin());
}
