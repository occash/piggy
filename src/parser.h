#pragma once

#include "lexer.h"
#include "token.h"
#include "type.h"
#include "map.h"
#include "ast.h"

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

		ast::noderef parse();

    private:
        token get();
		token peek();
		void unget(token t);

		bool is_type(token &t);
		ast::noderef parse_declaration();
        float parse_number(const char *p, const char **q);
        const char *parse_string(const char *p, const char **q, char delim);

    private:
        lexer &m_lexer;
		std::vector<token> m_buffer; // TODO: vector for only one token?
		map<type> m_types;

    };
}