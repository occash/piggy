#include "lexer.h"

#include <regex>
#include <cstring>

const char *keywords[] = {
    "def", "if", "else", "false", "true"
};

float parse_number(const char *p, const char **q)
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

const char *parse_string(const char *p, const char **q, char delim)
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

namespace piggy
{
    lexer::lexer(const char *text) :
        m_cursor(text)
    {
    }

    bool lexer::tokenize(std::vector<token>& tokens, const char **error)
    {
        const char *current = m_cursor;

        for (;;)
        {
            switch (*current)
            {
            case '\0':
                tokens.push_back({ token::type::eof });
                return true;
            case '\n': case '\r':
                ++current; // TODO: use line numbers?
                break;
            case ' ':
            case '\f':
            case '\t':
            case '\v':
                // Skip spaces
                ++current;
                break;
            case '#':
                // TODO: use comment?
                while (*current && *current != '\n' && *current != '\r')
                    ++current;
                break;
            case '-':
            case '+':
            case '/':
            case '*':
            case '=':
                tokens.push_back({ token::type::reserved, int(*current++) });
                break;
            case '\'': case '"':
            {
                const char *result = parse_string(current, &current, *current);
                tokens.push_back({ token::type::string, result });
                break;
            }
            case '.': // TODO: member access
            case '0': case '1': case '2': case '3': case '4':
            case '5': case '6': case '7': case '8': case '9':
            {
                // Read number
                float result = parse_number(current, &current);
                tokens.push_back({ token::type::number, result });
                break;
            }
            default:
            {
                // Read the rest
                if ((*current >= 'A' && *current <= 'F') ||
                    (*current >= 'a' && *current <= 'f'))
                {
                    std::vector<char> result;
                    char *value;

                    while ((*current >= 'A' && *current <= 'F') ||
                        (*current >= 'a' && *current <= 'f') ||
                        (*current >= '0' && *current <= '9'))
                        result.push_back(*current++);

                    value = new char[result.size() + 1];
                    std::memcpy((void *)value, result.data(), result.size() * sizeof(char));
                    value[result.size()] = '\0';

                    tokens.push_back({ token::type::name, value });
                }
                else
                {
                    // TODO: lex error
                }
                break;
            }
            }
        }

        return true;
    }
}