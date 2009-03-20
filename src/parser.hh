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

class MapParser
{
    const Glib::ustring _filename; ///< name of the file we'll load.
    public:
    /// Construct a new parser that will parse the map called [filename].
    ///@param filename name of the file (not the full path) to load.
    MapParser (const Glib::ustring& filename = "example.xml") :
        _filename (filename) {};

    /// Process the XML into a usable Map object.
    Map* parse () const;
};


typedef const xmlpp::Node::NodeList::const_iterator xmlmapIter;

#endif
