#pragma once

namespace piggy
{
    class token
    {
    public:
        enum class type
        {
            eof,
            space,
            comment,
            number,
            keyword,
            identifier
        };

        type t;
        union {
            int k; //keyword
            const char *s; //string
        };

        token(type t) :
            t(t) {}
        token(type t, const char *s) :
            t(t), s(s) {}
        token(type t, int o) :
            t(t), k(o) {}
    };
}