#ifndef JOURNEY
#define JOURNEY
#include "party.hh"
#include "world.hh"

/// Interface for moving the party
class Journey
{
    private:
    static Party* the_party;

    public:
    static void init(Party* _party);
};

#endif
