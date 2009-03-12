#ifndef COMMON
#define COMMON

enum weather {CLOUDY, PARTLY_CLOUDY, SUNNY, RAINY};

#ifndef NDEBUG
#include <glibmm.h>

namespace Freetrail
{
    void Debug(const Glib::ustring& msg);
}
#endif //NDEBUG

#endif //COMMON
