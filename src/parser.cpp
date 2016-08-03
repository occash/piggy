#include "parser.h"
#include "error.h"

#include <vector>

piggy::map<piggy::type>::item builtin[]{
    { "any", { 16, 4, false } },
    { "int", { 8, 4, false } },
    { "uint", { 8, 4, true } },
    { "float", { 8, 4, false } },
    { "double", { 16, 4, false } },
    { "vec2", { 16, 16, false } },
    { "vec3", { 32, 16, false } },
    { "vec4", { 32, 16, false } },
    { "mat3", { 96, 16, false } },
    { "mat4", { 128, 16, false } },
};

namespace piggy
{
    parser::parser(lexer &lex) :
        m_lexer(lex),
        m_types(builtin)
    {
    }

    ast::noderef parser::parse()
    {
        ast::ref<ast::scope> globals{ new ast::scope() };

        while (true)
        {
            token t = peek();
            if (t.kind == token::type::eof)
                break;

            switch (t.kind)
            {
            case token::type::identifier:
            {
                ast::noderef decl = parse_decl();
                globals->add(decl);
                break;
            }
            default:
                throw syntax_error{ "Unexpected token" };
            }
        }

        return ast::ref_cast<ast::node>(globals);
    }

    token parser::get()
    {
        if (m_buffer.size() > 0) {
            token value = m_buffer.back();
            m_buffer.pop_back();
            return value;
        }

        return m_lexer.get();
    }

    token parser::peek()
    {
        if (m_buffer.size() > 0)
            return m_buffer.back();

        token t = m_lexer.get();
        m_buffer.push_back(t);
        return t;
    }

    void parser::unget(token t)
    {
        m_buffer.push_back(t);
    }

    bool parser::is_type(token &t)
    {
        if (t.kind != token::type::identifier)
            return false;

        return m_types.check(t.id);
    }

    bool parser::is_keyword(token & t, keyword k)
    {
        if (t.kind != token::type::keyword)
            return false;

        return t.term == k;
    }

    ast::noderef parser::parse_decl()
    {
        token var = get();        
        type t = m_types.get("any");

        if (is_type(peek()))
            t = m_types.get(get().id);

        auto decl = new ast::decl{};
        decl->kind = ast::type::decl;
        decl->name = var.id;
        decl->type = t;

        if (is_keyword(peek(), keyword::kassign))
        {
            get();
            decl->init = parse_assign();
        }

        return ast::noderef{ decl };
    }

    ast::noderef parser::parse_assign()
    {
        auto init = new ast::init{};
        init->kind = ast::type::init;

        token val = get();
        if (val.kind != token::type::number)
            throw syntax_error{ "Number expected" };

        return ast::noderef{ init };
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