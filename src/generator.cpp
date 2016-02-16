#include "generator.h"
#include "helper.h"

#include <sstream>

namespace piggy
{
	generator::generator(std::ostream &out) :
		m_output(out)
	{
	}

	void generator::generate(ast::noderef &root)
	{
        std::stringstream bss;

        // Sort global declarations
		auto globals = ast::ref_cast<ast::scope>(root);

		for (auto i = globals->locals.begin(); i != globals->locals.end(); ++i)
		{
            if ((*i)->kind == ast::type::decl)
            {
                auto decl = ast::ref_cast<ast::decl>(*i);
                auto label = decl->name.c_str();
                auto size = decl->type.size;
                bss << emit(string::format("%s: resb %d", label, size), 1);
            }
		}

        write("segment .bss");
        write(bss.str());

        write("segment .text");
        write("global main", 1);
        write("main:");
        write("mov eax, 0", 1);
        write("ret", 1);
	}

    std::string generator::emit(const std::string &cmd, int ident)
    {
        if (ident > 0)
            return string::format("%*c%s\n", ident * 4, ' ', cmd.c_str());

        return string::format("%s\n", cmd.c_str());
    }

	void generator::write(const std::string &cmd, int ident)
	{
		std::string c = emit(cmd, ident);
		m_output.write(c.c_str(), c.length());
	}

    void generator::emit_decl(ast::decl *)
    {
    }
}
