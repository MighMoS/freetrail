#ifndef UI
#define UI

#include <glibmm.h>

#include "location.hh"
#include "party.hh"

const char horizrule[] = "--------------------------------------------------------------------------------\n";
const char select_one[] = "Select an option: ";

class user_interface
{
	public:
	static void clear_screen ();
    /// Creates a Party for the player to use.
    static Party* init_party ();
    /// Message to be displayed when the user loses.
    static void lose ();
    /// Allows the user to choose which ForkOption to take.
    static const ForkOption& getForkChoice (const Fork& loc);
    /// Informs the player that some party members couldn't eat.
    static void displayStarvedMembers (const MemberContainer& hungry_members);
    /// Allows the Party to purchase additional supplies.
    static void shop (Party& party);
    /// Annouces to the user that we've arrived.
    static void travel_path_finish (const Glib::ustring& where);
    /// Tells the user what's happened while journying.
    static void travel_path_summery (const Party& party,
            const unsigned int speed, const unsigned int total);
	static void wait_for_key ();
    /// Intro dialog.
    static void welcome_new_user ();
    /// Message to be displayed when the user wins.
    static void win (const Party& party);
};
#endif
