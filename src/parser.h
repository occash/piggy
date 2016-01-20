#pragma once

#include "token.h"
#include "lexer.h"

namespace piggy
{
    class parser
    {
    public:
        parser(lexer &lex);

        void parse();

    private:
        token get();
        float parse_number(const char *p, const char **q);
        const char *parse_string(const char *p, const char **q, char delim);

    private:
        lexer &m_lexer;

    };
}