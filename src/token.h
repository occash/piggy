#pragma once

#include <string>

namespace piggy
{
    enum class keyword
    {
        kdef,
        kif,
        kelse,

        //binops
        kassign
    };

    class token
    {
    public:
        enum class type
        {
			invalid,
            eof,
            space,
            comment,
            number,
            keyword,
            identifier
        };

        type kind;
		keyword term;
		std::string id;

		token() :
			kind(type::invalid) {}
        token(type t) :
            kind(t) {}
        token(type t, const std::string& i) :
            kind(t), id(i) {}
        token(type t, keyword k) :
            kind(t), term(k) {}
    };
}