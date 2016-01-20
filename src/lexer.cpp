#include "lexer.h"

#include <cstdio>
#include <cstring>


bool is_space(char c)
{
    return c == ' ' || c == '\f' || c == '\t' || c == '\v';
}

bool is_digit(char c)
{
    return c >= '0' && c <= '9';
}

bool is_alpha(char c)
{
    return (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}

namespace piggy
{
    lexer::lexer(std::istream &source) :
        m_source(source)
    {
    }

    token lexer::get()
    {
        int current = read();

        switch (current)
        {
        case EOF:
            return{ token::type::eof };
        case '\n': case '\r':
            // TODO: use line numbers?
        case ' ':
        case '\f':
        case '\t':
        case '\v':
            // Skip spaces
            skip_space();
            return{ token::type::space };
        /*case '#':
            // TODO: use comment?
            while (current && *current != '\n' && *current != '\r')
                ++current;
            break;*/
        case '-':
        case '+':
        case '/':
        case '*':
        case '=':
            return{ token::type::keyword, int(current) };
        /*case '\'': case '"':
        {
            const char *result = parse_string(current, &current, *current);
            tokens.push_back({ token::type::string, result });
            break;
        }*/
        case '.': // TODO: member access
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            // Read number
            return read_number(current);
        default:
            return read_identifier(current);
        }
    }

    char lexer::read()
    {
        return m_source.get();
    }

    void lexer::unread(int n)
    {
        m_source.seekg(-n, m_source.cur);
    }

    void lexer::skip_space()
    {
        char current;
        do { current = read(); } 
        while (is_space(current));
        unread();
    }

    token lexer::read_number(char c)
    {
        std::string n;
        n.push_back(c);
        char last = c;

        for (;;)
        {
            char c = read();
            bool flonum = std::strchr("eEpPxX", last) && std::strchr("+-", c);

            if (!is_digit(c) && !is_alpha(c) && c != '.' && !flonum)
            {
                unread();

                int size = n.size();
                char *r = new char[size + 1];
                n.copy(r, size);
                r[size] = '\0';
                return{ token::type::number, r };
            }

            n.push_back(c);
            last = c;
        }
    }

    token lexer::read_identifier(char c)
    {
        if ((c >= 'a' && c <= 'z') ||
            (c >= 'A' && c <= 'Z') || 
            c == '_')
        {
            std::string n;
            n.push_back(c);
            
            c = read();
            while ((c >= 'a' && c <= 'z') ||
                   (c >= 'A' && c <= 'Z') ||
                   (c >= '0' && c <= '9') ||
                   c == '_')
            {
                n.push_back(c);
                c = read();
            }
            unread();

            int size = n.size();
            char *r = new char[size + 1];
            n.copy(r, size);
            r[size] = '\0';
            return{ token::type::identifier, r };
        }
        //else
        //Throw lex error
    }
}