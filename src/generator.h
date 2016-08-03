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

    protected:
        std::string emit(const std::string &cmd, int ident = 0);
        void write(const std::string &cmd, int ident = 0);

    protected:
        virtual void emit_decl(ast::decl *);

    private:
        std::ostream &m_output;

    };
}