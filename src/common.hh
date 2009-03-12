#ifndef COMMON
#define COMMON

enum sex_ {MALE, FEMALE};
enum health_ {DEAD, DYING, POOR, FAIR, GOOD, EXCELLENT};
enum weather {CLOUDY, PARTLY_CLOUDY, SUNNY, RAINY};

#ifndef NDEBUG
#include <glibmm.h>

namespace Freetrail
{
    void Debug(const Glib::ustring& msg);
}
#endif //NDEBUG

#endif //COMMON
