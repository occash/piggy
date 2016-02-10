#pragma once

#include "token.h"
#include "lexer.h"

namespace piggy
{
    class parser
    {
	public:
		class error
		{
		public:
			std::string message;
		};

    public:
        parser(lexer &lex);

        void parse();

    private:
        token get();
		token peek();
		void unget(token t);

		void parse_declaration();
        float parse_number(const char *p, const char **q);
        const char *parse_string(const char *p, const char **q, char delim);

    private:
        lexer &m_lexer;

    };
}