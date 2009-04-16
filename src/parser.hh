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
    const std::string _filepath; ///<Where to look

    public:
    /// Construct a new parser that will parse the map called [filename].
    ///@param filename name of the file (not the full path) to load.
    MapParser (const Glib::ustring& filename,
            const std::string& filepath = std::string ("./")) :
        _filename (filename), _filepath (filepath) {};

    /// Process the XML into a usable Map object.
    Map* parse () const;
};


typedef const xmlpp::Node::NodeList::const_iterator xmlmapIter;

#endif
