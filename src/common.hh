#ifndef COMMON_H
#define COMMON_H

enum weather {CLOUDY, PARTLY_CLOUDY, SUNNY, RAINY};

#include <glibmm.h>

namespace Freetrail
{
    void Debug (const Glib::ustring& msg);
}

#endif //COMMON_H
