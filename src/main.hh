#include <iostream>
#include <fstream>
#include <string>

#include <glibmm.h>

#include "config.h"

namespace Freetrail
{
    /// Class who's initialization will set logging to a file
    class IerrLog
    {
        std::streambuf * original_buffer;
        std::ofstream logFile;

        public:
        ///TODO: use libxdg-basedir: xdgCacheHome ();
        IerrLog ()
        {
            std::string freetrailLog ("freetrail.log");
            logFile.open (freetrailLog.c_str());
            original_buffer = std::clog.rdbuf (logFile.rdbuf ());
            std::clog << "Freetrail log, version " << PACKAGE_VERSION 
                << "\nSend bug reports to " << PACKAGE_BUGREPORT
                << "\n" << std::endl;
        }
        ~IerrLog ()
        {
            std::clog.rdbuf (original_buffer);
        }
    };

    /// Creating one of these will prepare the program to do
    /// everything "normally". Really just a wrapper class.
    
    /// I could have easily called it something clever like EverythingYoullEverNeed
    ///     (bonus points if you get the reference)
    class IEnvironment
    {
        private:
        IerrLog log;
        public:
        IEnvironment ()
        {
            Glib::set_prgname ("Freetrail");
        }
    };
}

