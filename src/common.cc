#include <iostream>

#include <glibmm.h>

#include "common.hh"

namespace Freetrail
{
    void Debug(const Glib::ustring& msg)
    {
        std::cerr << msg << std::endl;
    }
}
