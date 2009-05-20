#include <cassert>
#include <iostream>
#include <sstream>
#include <vector>

#include <glibmm.h>
#include <libxml++/libxml++.h>

#include "common.hh"
#include "location.hh"
#include "parser.hh"
#include "world.hh"


template <typename T>
static inline T convert_extraced_input (const xmlpp::TextNode* node_text)
{
    std::stringstream ss;
    T object;
    ss << node_text->get_content();
    ss >> object;
    return object;
}

template <>
inline Glib::ustring
convert_extraced_input<Glib::ustring> (const xmlpp::TextNode* node_text)
{
    Glib::ustring object (node_text->get_content());
    return object;
}

/**
 * I may have slightly over engineered this. Perhaps not, either way,
 it either does too much or not enough.
 */
template <typename T>
static std::vector<T> extract_tag_list (const xmlmapIter& iter,
        const Glib::ustring& name, const bool preserve = false)
{
    const xmlpp::Node::NodeList name_tag = (*iter)->get_children(name);
    const xmlpp::TextNode* node_text;
    xmlpp::Node::NodeList::const_iterator i = name_tag.begin();
    std::vector<T> found_items;

    for (; i != name_tag.end (); i++)
    {
        const xmlpp::Element* nodeElement =
            dynamic_cast<const xmlpp::Element*> (*(name_tag.begin()));

        if (nodeElement->has_child_text () == false)
            throw MapParsingException (name + " tag was empty.");

        node_text = nodeElement->get_child_text ();
        if (!node_text || node_text->is_white_space ())
            continue;
        found_items.push_back (convert_extraced_input<T>(node_text));
    }

    return found_items;
}

/**
 * Extracts the name tag from an element.
 * @param[in,out] iter an Iterator to extract name from.
 * @param name name of the element to look for.
 * @returns first element found matching description.
 *
 * @todo don't throw an exception, but allow things to be optional.
 */
template <typename T>
static T extract_tag (const xmlmapIter& iter, const Glib::ustring& name)
{
    std::vector<T> tags (extract_tag_list<T>(iter, name));
    if (tags.empty())
        throw MapParsingException (name + " tags was not found");
    return tags[0];
}

static CostContainer extract_costs (const xmlmapIter& iter)
{
    CostContainer costs;
    try
    {
    unsigned int amount = extract_tag<unsigned int> (iter, "money");
    costs.push_back (CostPtr (new CostsMoney (amount)));
    }
    catch (MapParsingException ex)
    {
        // Probably didn't find anything and that's okay.
    }

    return costs;
}

/**
 * Constructs an Outpost from iter
 * @returns a ready to use Outpost.
 * @relates Outpost
 */
static Outpost* fill_outpost (const xmlmapIter& iter)
{
    Glib::ustring outpost_name;

    outpost_name = extract_tag <Glib::ustring>(iter, "name");
    Outpost* loc = new Outpost (outpost_name);

    return loc;
}

///Creates a new location based off the XML pointed to by stop_iter
///@relates Path
/**
 *@verbatim <path><name>thisPathName</name><length>100</path> @endverbatim
 * would create a path with a length of 100 called "thisPathName"
 */
static Path*
fill_path (const xmlmapIter& iter, const Glib::ustring& type)
{
    xmlpp::Node::NodeList path_children;
    Glib::ustring path_name;
    Path* loc = NULL;
    unsigned int path_length;

    path_name = extract_tag <Glib::ustring>(iter, "name");
    path_length = extract_tag <unsigned int>(iter, "length");

    if (type == "path")
        loc = new Path (path_name, path_length);
    else if (type == "winningpath")
        loc = new WinningPath (path_name, path_length);
    else
    {
        assert (0 && "Path was neither a regular path or a winning one");
    }

    return loc;
}

///@relates ForkOption
static ForkOptionPtr fill_jump (const xmlmapIter& iter)
{
    Glib::ustring destination;
    Glib::ustring description;
    const xmlpp::Element* jump = dynamic_cast<const xmlpp::Element*> (*iter);
    const xmlpp::Attribute* jump_dest = jump->get_attribute(Glib::ustring("dest"));
    CostContainer costs;

    destination = jump_dest->get_value ();
    description = extract_tag <Glib::ustring>(iter, "name");
    costs = extract_costs (iter);

    ForkOptionPtr ptr (new ForkOption(description, destination, costs));
    return ptr;
}

///@relates Fork
/**
 *@param iter
 *@param type indicates if we are a regular Fork or a FixedJump.
 */
static Fork*
fill_fork (const xmlmapIter& iter, const Glib::ustring& type)
{
    xmlpp::Node::NodeList fork_children;
    Glib::ustring fork_name;
    ForkOptionContainer option_list;
    Fork* fork = NULL;

    fork_name = extract_tag <Glib::ustring>(iter, "name");
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
///@relates Location
static LocationPtr fill_location (const xmlmapIter& iter)
{
    Glib::ustring type;
    LocationPtr loc;

    type = (*iter)->get_name ();

    if (type == "path" || type == "winningpath")
        loc.reset (fill_path (iter, type));
    else if (type == "outpost")
        loc.reset (fill_outpost (iter));
    else if (type == "userjump" || type == "fixedjump")
        loc.reset (fill_fork (iter, type));

    return loc;
}

/// Returns a complete, ready to use track
/**
 * @param track_iter iterator of who's children we will construct
 * the track from.
 * @todo use exceptions to raise errors about what we expected in the
 * document
 */
static inline Track fill_track (const xmlmapIter& track_iter)
{
    xmlpp::Node::NodeList track_list = (*track_iter)->get_children ();
    Glib::ustring name (extract_tag <Glib::ustring>(track_iter, "name"));
    Track new_track (name);

    std::clog << "\tCreated track: " << name << std::endl;;

    // Get all  our children
    for(xmlpp::Node::NodeList::const_iterator
            track_iter = track_list.begin();
            track_iter != track_list.end(); track_iter++)
    {
        if ((*track_iter)->get_name () == "text") continue;
        LocationPtr loc = fill_location (track_iter);
        if (loc != NULL)
        {
            new_track.add_location (loc);
            std::clog << "\t\tFilled location: " << loc->get_name() << std::endl;
        }
    }

    return new_track;
}

/// Gets the full path for filename
/**
 * @param filename filename to look for, ex "example.xml"
 * @param additional_path pathname for location of the file (optional)
 * @returns full path to the file name
 * @throws MapParsingException if the file couldn't be found.
 *
 * @todo Check to see if filename is a complete path
 */
static inline std::string
find_map_file (const std::string& filename, const std::string& additional_path)
{
    static const std::string data_dir =
        Glib::get_user_data_dir () + "/freetrail";
    // The following is defined in configure.ac
    static const std::string file_path (FREETRAIL_MAPSDIR);
    // Places to look for the given filename
    static const std::string search_paths[] =
        {additional_path, data_dir, file_path};
    std::string complete_file_name;
    bool really_exists = false;

    for (unsigned int i = 0;
            i < sizeof(search_paths)/sizeof(search_paths[0]) &&
            really_exists == false; i++)
    {
        complete_file_name = Glib::build_filename (search_paths[i], filename);
        std::clog << "Checking " << complete_file_name << std::endl;
        really_exists =
            Glib::file_test(complete_file_name, Glib::FILE_TEST_EXISTS);
    }
    if (really_exists == false)
        throw MapParsingException ("Could not find map called " + filename);

    return complete_file_name;
}

/**
 *@param filename name of the file (not the full path) to load.
 *@param filepath additional path to search for @c filename
 */
MapParser::MapParser (const Glib::ustring& filename,
        const std::string& filepath) :
    _filename (filename), _filepath (filepath)
{
}

/**
 @returns a pointer to a newly allocated Map object.
 @todo Check that all destinations are reachable
*/
const Map MapParser::parse () const
{
    TrackContainer all_tracks;
    xmlpp::DomParser parser;
    xmlpp::Node::NodeList track_list;
    const xmlpp::Element* root_element;
    const xmlpp::Attribute* starting_track;
    Glib::ustring first_track;
    const std::string complete_file_name =
        find_map_file (_filename, _filepath);

    std::clog << "Loading file: " << complete_file_name << std::endl;

    // Swallow any error libxml++ gives us and rethrow it.
    try
    {
        parser.parse_file (complete_file_name);
        parser.set_validate ();
        parser.set_substitute_entities ();
    }
    catch (const std::exception& ex)
    {
        MapParsingException e (std::string("error parsing XML:\n\t") +
                ex.what ());
        throw e;
    }

    // Parse the document
    const xmlpp::Node* rNode = parser.get_document()->get_root_node();
    if (rNode->get_name() != "freetrail")
        throw MapParsingException ("expected root node to be \"freetrail\"!");

    root_element = dynamic_cast<const xmlpp::Element*>(rNode);
    starting_track = root_element->get_attribute (Glib::ustring("start"));
    first_track = starting_track->get_value ();
    assert ("" != first_track);

    track_list = rNode->get_children("track");

    // Cannot wait for the auto keyword
    // Anyway, hop through all the tracks for each of their shits
    for (xmlpp::Node::NodeList::iterator track_iter = track_list.begin ();
            track_iter != track_list.end (); track_iter++)
    {
        all_tracks.insert (all_tracks.begin (), fill_track (track_iter));
    }

    // Really there does need to be a better sanity check here.
    for (TrackContainer::iterator i = all_tracks.begin (); i != all_tracks.end (); i++)
    {
        Freetrail::Debug ("Sanity check:");
        Freetrail::Debug ("\tAdded " + i->get_name ());
        Freetrail::Debug ("\tCan reference " +
                (all_tracks.find (i->get_name ()))->get_name());
        if ((all_tracks.find (i->get_name ())) == all_tracks.end ())
            throw MapParsingException ("The map failed its sanity check (" +
                    i->get_name () + ")");
    }

    return Map (first_track, all_tracks);
}

