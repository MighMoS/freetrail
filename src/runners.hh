#ifndef RUNNERH
#define RUNNERH

#include "party.hh"
#include "world.hh"

namespace Freetrail{
    namespace Runner
{
    /// What is the current state of the game?
    /// Have we won, lost, or do we keep playing?
    enum state {KEEP_RUNNING, WIN, LOSE};

    /// Container holding what happened, and where to go.
    class Status
    {
        const Track* _next_track;
        state _state;

        public:
        Status ();
        void setNextTrack (const Track* track);
        void setStatus (const state state);

        const Track* getNextTrack () const {return _next_track;};
        bool KeepRunning () const {return _state == KEEP_RUNNING;};
        bool hasWon () const {return _state == WIN;};
        bool hasLost () const {return _state == LOSE;};
    };

/// Abstract class for moving the party through places.
/// @todo: Can this be done with templates?
class Runner
{
    protected:
        const Party* _party;
    public:
        Runner (const Party* _party);
        virtual Status run() = 0;
};

/// Runs a party through the whole map.
class IMapRunner : public Runner
{
    const Map* _map;
    const Track* _curr_track;

    public:
    IMapRunner (const Party* party, const Map* map);
    Status run();
};

/// Runs a party through a single track.
class ITrackRunner : public Runner
{
    const Track* _track;
    const Location* _curr_location;

    public:
    ITrackRunner (const Party* party, const Track* track);
    Status run();
};

/// Runs a party through a single location.
/// This is where most of the work is done.
class ILocationRunner : public Runner
{
    const Location* _location;

    public:
    ILocationRunner (const Party* party, const Location* location);
    Status run();
};

} // namespace Runner
} // namespace Freetrail

#endif // RUNNERH
