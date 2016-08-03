#include "ast.h"
#include "lexer.h"
#include "parser.h"
#include "generator.h"
#include "error.h"

#include <iostream>
#include <sstream>

int main(int argc, char *argv[])
{
    const char *script = "a int lolo\nb any";
    std::stringstream source(script);

    piggy::lexer lexer{ source };
    piggy::parser parser{ lexer };
    piggy::generator generator{ std::cout };

    try
    {
        piggy::ast::noderef tree = parser.parse();
        generator.generate(tree);
    }
    catch (piggy::lexical_error e)
    {
        std::cout << e.message << std::endl;
    }
    catch (piggy::syntax_error e)
    {
        std::cout << e.message << std::endl;
    }

    return 0;
}