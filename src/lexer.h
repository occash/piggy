#pragma once

#include <vector>

namespace piggy
{
    class token
    {
    public:
        enum operation
        {
            equal,
            greater,
            less
        };

        enum class type
        {
            eof,
            keyword,
            name,
            reserved,
            number,
            string,
            comment
        };

        type ttype;
        union {
            const char *tstring;
            float tnumber;
            int toperator;
        };

        token(type t) :
            ttype(t) {}
        token(type t, const char *s) : 
            ttype(t), tstring(s) {}
        token(type t, float n) :
            ttype(t), tnumber(n) {}
        token(type t, int o) :
            ttype(t), toperator(o) {}
    };

    class lexer
    {
    public:
        lexer(const char *text);

        //float parse_number(const char *p, const char **q = nullptr);
        bool tokenize(std::vector<token>& tokens, const char **error = nullptr);

    private:
        const char *m_cursor;
    };

}