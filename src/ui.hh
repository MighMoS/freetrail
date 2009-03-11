#ifndef UI
#define UI

#include <vector>

#include "location.hh"
#include "party.hh"

class user_interface
{
	public:
	static void clear_screen();
    /// Message to be displayed when the user loses.
    static void lose ();
    /// Allows the user to choose which ForkOption to take.
    static const ForkOption* prompt_at_fork (const Fork& loc);
    /// Allows the Party to purchase additional supplies.
    static void shop(Party& party);
	static void wait_for_key();
    /// Intro dialog.
    static void welcome_new_user();
    /// Message to be displayed when the user wins.
    static void win(const Party& party);
};
#endif
