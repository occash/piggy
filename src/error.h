#pragma once

#include <string>

namespace piggy
{
    class lexical_error
    {
    public:
        std::string message;
        unsigned int line;
        unsigned int column;
    };

    class syntax_error
    {
    public:
        std::string message;
    };
}