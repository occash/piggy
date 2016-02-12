#include "generator.h"
#include "helper.h"

namespace piggy
{
	generator::generator(std::ostream &out) :
		m_output(out)
	{
	}

	void generator::generate(ast::noderef &root)
	{
		emit("segment .data");

		ast::ref<ast::scope> globals = ast::ref_cast<ast::scope>(root);
		for (auto i = globals->locals.begin(); i != globals->locals.end(); ++i)
		{
			ast::ref<ast::declaration> decl = ast::ref_cast<ast::declaration>(*i);
			std::string cmd = string::format("%s: dd 0", decl->name.c_str());
			emit(cmd, 1);
		}

		emit("segment .text");
		emit("global main", 1);
		emit("main:");
		emit("mov eax, 0", 1);
		emit("ret", 1);
	}

	void generator::emit(const std::string &cmd, int ident)
	{
		std::string c = string::format("%*c%s\n", ident * 4, ' ', cmd.c_str());
		m_output.write(c.c_str(), c.length());
	}
}
