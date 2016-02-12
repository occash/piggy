#pragma once

#include "ast.h"

#include <ostream>

namespace piggy
{
	class generator
	{
	public:
		generator(std::ostream &out);
		virtual ~generator() {}

		void generate(ast::noderef &root);

	private:
		void emit(const std::string &cmd, int ident = 0);

	private:
		std::ostream &m_output;

	};
}