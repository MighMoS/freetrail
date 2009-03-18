#ifndef PARSERH
#define PARSERH
#include <stdexcept>
#include <string>

#include <glibmm.h>
#include <libxml++/libxml++.h>

#include "location.hh"
#include "world.hh"

class MapParsingException : public std::runtime_error
{
    public:
        MapParsingException (const std::string& message) :
            std::runtime_error(message)
    {};
};

typedef const xmlpp::Node::NodeList::const_iterator xmlmapIter;

Map* fill_map (const std::string& filename);

Outpost* fill_outpost (xmlmapIter& iter);

Path* fill_path (xmlmapIter& stop_iter, const Glib::ustring& type);

ForkOptionPtr fill_jump (xmlmapIter& iter);

Fork* fill_fork (xmlmapIter& iter, const Glib::ustring& type);

LocationPtr fill_location (xmlmapIter& iter);

Track* fill_track (xmlmapIter& track_iter);

#endif
