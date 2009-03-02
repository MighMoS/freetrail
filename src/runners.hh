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
        const Track* _next_track; ///<Where to go to next. If this is NULL, allow controller to decide.
        state _state; ///<Win, Lose, or neither yet?

        public:
        Status ();
        /// Used to jump to another track.
        void setNextTrack (const Track* track);
        /// Used to indicate success or failure.
        void setStatus (const state state);

        const Track* getNextTrack () const {return _next_track;};
        /// Do we keep going?
        bool KeepRunning () const {return _state == KEEP_RUNNING;};
        /// Have we won?
        bool hasWon () const {return _state == WIN;};
        /// Have we lost?
        bool hasLost () const {return _state == LOSE;};
    };

/// Abstract class for moving the party through places.
/// @todo: Can this be done with templates?
class Runner
{
    protected:
        const Party* _party; ///<The party we're running.
    public:
        Runner (const Party* _party);
        virtual Status run() = 0;
};

/// Runs a Party through the whole Map.
class IMapRunner : public Runner
{
    const Map* _map;
    const Track* _curr_track;

    public:
    IMapRunner (const Party* party, const Map* map);
    /// Plays the game. Won't stop until there is no where else to go.
    Status run();
};

/// Runs a Party through a single Track.
class ITrackRunner : public Runner
{
    const Track* _track;
    const Location* _curr_location;

    public:
    ITrackRunner (const Party* party, const Track* track);
    /// Moves through an area.
    Status run();
};

/// Runs a Party through a single Location.
class ILocationRunner : public Runner
{
    const Location* _location;

    public:
    ILocationRunner (const Party* party, const Location* location);
    /// Moves through a single Location, as defined by its subtype.
    Status run();
};

} // namespace Runner
} // namespace Freetrail

#endif // RUNNERH
