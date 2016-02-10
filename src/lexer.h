#pragma once

#include <istream>
#include <vector>

#include "token.h"

namespace piggy
{
    class lexer
    {
    public:
        class error
        {
        public:
            std::string message;
            unsigned int line;
            unsigned int column;
        };

    public:
        lexer(std::istream &source);

		token peek();
        token get();
		void unget(token t);

    public:
        static const unsigned int IDENT_SIZE = 64;

    private:
        int cpeek();
        int read();
        void unread(int n = 1);

		token current();
        void skip_space();
        token read_number(int c);
        token read_identifier(int c);

    private:
        std::istream &m_source;
        unsigned int m_line;
        unsigned int m_column;
		std::vector<token> m_buffer;

    };
}