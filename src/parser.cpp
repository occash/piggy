#include "parser.h"
#include "ast.h"

#include <vector>

using type = piggy::token::type;

namespace piggy
{
    parser::parser(lexer &lex) :
        m_lexer(lex)
    {
    }

    void parser::parse()
    {
		token t = peek();
        while (true)
        {
			switch (t.kind)
			{
			case type::eof:
				return;
			case type::identifier:
				parse_declaration();
				return;
			default:
				throw error{"Unexpected token"};
			}
        }
    }

    piggy::token parser::get()
    {
        return m_lexer.get();
    }

	token parser::peek()
	{
		return m_lexer.peek();
	}

	void parser::unget(token t)
	{
		m_lexer.unget(t);
	}

	void parser::parse_declaration()
	{

	}

    float parser::parse_number(const char *p, const char **q)
    {
        float value = 0;
        if (*p == '0' && (p[1] == 'x' || p[1] == 'X'))
        {
            p += 2;
            while (*p)
            {
                if (*p >= '0' && *p <= '9')
                    value = value * 16 + (*p++ - '0');
                else if (*p >= 'a' && *p <= 'f')
                    value = value * 16 + (*p++ - 'a') + 10;
                else if (*p >= 'A' && *p <= 'F')
                    value = value * 16 + (*p++ - 'A') + 10;
                else
                    break;
            }
            if (*p == '.')
            {
                float powhex = 1, addend = 0;
                ++p;
                while (*p)
                {
                    if (*p >= '0' && *p <= '9')
                        addend = addend * 16 + (*p++ - '0');
                    else if (*p >= 'a' && *p <= 'f')
                        addend = addend * 16 + ((*p++ - 'a') + 10);
                    else if (*p >= 'A' && *p <= 'F')
                        addend = addend * 16 + ((*p++ - 'A') + 10);
                    else
                        break;

                    powhex *= 16;
                }
                value += addend / powhex;
            }
            if (*p == 'p' || *p == 'P')
            {
                int sign = p[1] == '-';
                int exponent = 0;
                float pow2 = 1;
                p += 1 + sign;
                while (*p)
                {
                    if (*p >= '0' && *p <= '9')
                        exponent = exponent * 16 + (*p++ - '0');
                    else if (*p >= 'a' && *p <= 'f')
                        exponent = exponent * 16 + (*p++ - 'a') + 10;
                    else if (*p >= 'A' && *p <= 'F')
                        exponent = exponent * 16 + (*p++ - 'A') + 10;
                    else
                        break;
                }
                while (exponent-- > 0)
                    pow2 *= 2;
                if (sign)
                    value /= pow2;
                else
                    value *= pow2;
            }
        }
        else
        {
            while (*p >= '0' && *p <= '9')
                value = value * 10 + (*p++ - '0');
            if (*p == '.')
            {
                float powten = 1, addend = 0;
                ++p;
                while (*p >= '0' && *p <= '9')
                {
                    addend = addend * 10 + (*p++ - '0');
                    powten *= 10;
                }
                value += addend / powten;
            }
            if (*p == 'e' || *p == 'E')
            {
                int sign = p[1] == '-';
                int exponent = 0;
                float pow10 = 1;
                p += 1 + sign;
                while (*p >= '0' && *p <= '9')
                    exponent = exponent * 10 + (*p++ - '0');
                // can't use pow() from stdlib, so do it slow way
                while (exponent-- > 0)
                    pow10 *= 10;
                if (sign)
                    value /= pow10;
                else
                    value *= pow10;
            }
        }

        if (q) *q = p;
        return value;
    }

    const char *parser::parse_string(const char *p, const char **q, char delim)
    {
        std::vector<char> result;
        char *value = nullptr;

        while (*++p != delim)
        {
            char c;
            switch (*p)
            {
            case '\0': case '\n': case '\r':
                // TODO: lex error
                break;
            case '\\': { // Escape sequence
                switch (p[1])
                {
                case 'a': c = '\a'; break;
                case 'b': c = '\b'; break;
                case 'f': c = '\f'; break;
                case 'n': c = '\n'; break;
                case 'r': c = '\r'; break;
                case 't': c = '\t'; break;
                case 'v': c = '\v'; break;
                    //case 'x': c = '\x'; break; TODO: read hex
                    //case 'u': c = '\u'; break; TODO: read utf-8
                case '\n': case '\r':
                    c = '\n'; break; //TODO: use line number?
                case '\\': c = '\\'; break;
                case '\0': break; // TODO: lex error
                default:
                    // TODO: lex error (invalid escape sequense)
                    break;
                }
            }
            default:
                c = *p;
                break;
            }
            result.push_back(c);
        }

        value = new char[result.size() + 1];
        std::memcpy((void *)value, result.data(), result.size() * sizeof(char));
        value[result.size()] = '\0';

        if (q) *q = ++p;
        return value;
    }
}