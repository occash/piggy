#include "lexer.h"
#include "helper.h"

#include <regex>

piggy::map<piggy::keyword>::item keywords[] {
    { "def", piggy::keyword::kdef },
    { "if", piggy::keyword::kif },
    { "else", piggy::keyword::kelse }
};

inline bool is_eof(int c)
{
    return c == EOF;
}

inline bool is_space(char c)
{
    return c == '\n' || c == '\r' || 
           c == ' ' || c == '\f' || 
           c == '\t' || c == '\v';
}

inline bool is_digit(char c)
{
    return c >= '0' && c <= '9';
}

inline bool is_hex(char c)
{
    return (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}

inline bool is_alpha(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

namespace piggy
{
    lexer::lexer(std::istream &source) :
        m_source(source),
        m_line(0),
        m_column(0),
        m_keywords(keywords)
    {
    }

    token lexer::get()
    {
		skip_space();

		int current = read();

		switch (current)
		{
		case EOF:
			return{ token::type::eof };
			/*case '\n': case '\r':
			// TODO: use line numbers?
			case ' ':
			case '\f':
			case '\t':
			case '\v':
			// Skip spaces
			skip_space();
			return{ token::type::space };*/
			/*case '#':
			// TODO: use comment?
			while (current && *current != '\n' && *current != '\r')
			++current;
			break;*/
			/*case '-':
			case '+':
			case '/':
			case '*':*/
		case '=':
			return{ token::type::keyword, static_cast<keyword>(current) };
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

    int lexer::peek()
    {
        return m_source.peek();
    }

    int lexer::read()
    {
        int c = m_source.get();
        if (c == '\n')
        {
            m_line++;
            m_column = 0;
        }
        else
            m_column++;

        return c;
    }

    void lexer::unread(int n)
    {
        // Cannot move more than one line back!
        m_source.seekg(-n, m_source.cur);
        m_column -= n;
    }

	void lexer::skip_space()
    {
        int current;
        do { current = read(); } 
        while (is_space(current));
        unread();
    }

    token lexer::read_number(int c)
    {
        std::vector<char> number;
        number.reserve(IDENT_SIZE);

        auto read_digits = [this, &number, &c]()
        {
            while (is_digit(c))
            {
                number.push_back(c);
                c = read();
            }
        };

        auto read_hex_digits = [this, &number, &c]()
        {
            while (is_digit(c) || is_hex(c))
            {
                number.push_back(c);
                c = read();
            }
        };

        int n = peek();
        if (c == '0' && (n == 'x' || n == 'X'))
        {
            number.push_back(c);
            number.push_back(read());
            c = read();

            read_hex_digits();
            if (c == '.')
            {
                c = read();
                read_hex_digits();
            }
            if (c == 'p' || c == 'P')
            {
                c = read();
                if (c == '-')
                {
                    number.push_back(c);
                    c = read();
                }
                read_hex_digits();
            }
        }
        else
        {
            read_digits();
            if (c == '.')
            {
                c = read();
                read_digits();
            }
            if (c == 'e' || c == 'E')
            {
                c = read();
                if (c == '-')
                {
                    number.push_back(c);
                    c = read();
                }
                read_digits();
            }
        }

        unread();

        if (!is_space(c) && !is_eof(c))
            throw error{ string::format("Unknown character '%c'", char(c)), m_line, m_column };

		std::string id(number.begin(), number.end());

        return{ token::type::number, id };
    }

    token lexer::read_identifier(int c)
    {
        if (is_alpha(c) || c == '_')
        {
            std::vector<char> ident;
            ident.reserve(IDENT_SIZE);
            
            do
            {
                if (ident.size() >= IDENT_SIZE - 1)
                    throw error{ "Identifier is too long", m_line, m_column };

                ident.push_back(c);
                c = read();
            } while (is_alpha(c) || is_digit(c) || c == '_');
            
            unread();
            
            if (!is_space(c) && !is_eof(c))
                throw error{ string::format("Unknown character '%c'", char(c)), m_line, m_column };

			std::string id(ident.begin(), ident.end());

            // Test if keyword
            if (m_keywords.check(id))
                return{ token::type::keyword, m_keywords.get(id) };

            return{ token::type::identifier, id };
        }
        
		throw error{ string::format("Unknown character '%c'", char(c)), m_line, m_column };
    }
}