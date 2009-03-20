#include <iostream>

#include <glibmm.h>

#include "common.hh"

namespace Freetrail
{
    void Debug(const Glib::ustring& msg)
    {
#ifndef NDEBUG
        std::cerr << msg << std::endl;
#endif
    }
}
