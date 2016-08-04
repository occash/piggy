#pragma once

#include <istream>
#include <vector>
#include <map>

#include "token.h"

namespace piggy
{
    class lexer
    {
    public:
        lexer(std::istream &source);

        token get();

    public:
        static const unsigned int IDENT_SIZE = 64;

    private:
        int peek();
        int read();
        void unread(int n = 1);

        void skip_space();
        token read_number(int c);
        token read_identifier(int c);

    private:
        std::istream &m_source;
        unsigned int m_line;
        unsigned int m_column;
        std::map<std::string, keyword> m_keywords;

    };
}