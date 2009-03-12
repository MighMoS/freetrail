#include <stdexcept>
#include <string>

class MapParsingException : public std::runtime_error
{
    public:
        MapParsingException (const std::string& message) :
            std::runtime_error(message)
    {};
};
