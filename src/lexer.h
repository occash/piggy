#pragma once

#include <istream>

#include "token.h"

namespace piggy
{
    class lexer
    {
    public:
        lexer(std::istream &source);

        token get();

    private:
        char read();
        void unread(int n = 1);

        void skip_space();
        token read_number(char c);
        token read_identifier(char c);

    private:
        std::istream &m_source;

    };
}