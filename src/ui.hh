#ifndef UI
#define UI

#include "party.hh"

class user_interface
{
	public:
	static void clear_screen();
    static void shop(Party& party);
	static void wait_for_key();
    static void welcome_new_user();
    static void win(const Party& party);
};
#endif
