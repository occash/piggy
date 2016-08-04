#include "args.h"
#include "ast.h"
#include "lexer.h"
#include "parser.h"
#include "generator.h"
#include "error.h"

#include <iostream>
#include <fstream>


piggy::args::action_flag actions[]{
    { "help", "--help", "-h", "Get help", false }
};

piggy::args::value_flag values[]{
    { "output", "--output", "-o", "Output file", "out.asm" }
};

int main(int argc, char *argv[])
{
    piggy::args::parser args{ argc, argv };

    args.setup(actions);
    args.setup(values);

    if (!args.parse() || args.size() < 1) {
        args.print();
        return 1;
    }

    std::ifstream source{ args.value(1) };
    if (!source.good()) {
        std::cout << "Cannot open file " << args.value(1) << std::endl;
        return 2;
    }

    piggy::lexer lexer{ source };
    piggy::parser parser{ lexer };

    piggy::ast::noderef tree;

    try
    {
        tree = parser.parse();
    }
    catch (piggy::lexical_error e)
    {
        std::cout << e.message << std::endl;
        return 3;
    }
    catch (piggy::syntax_error e)
    {
        std::cout << e.message << std::endl;
        return 4;
    }

    std::ofstream output{ args.value("output") };
    if (!output.good()) {
        std::cout << "Cannot open file " << args.value("output") << std::endl;
        return 2;
    }

    piggy::generator generator{ output };
    generator.generate(tree);

    return 0;
}