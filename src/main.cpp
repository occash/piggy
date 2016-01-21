#include "lexer.h"
#include "parser.h"

#include <iostream>
#include <sstream>

int main(int argc, char *argv[])
{
    const char *script = "_value int = 1.55";
    std::stringstream source(script);

    piggy::lexer lexer(source);
    piggy::parser parser(lexer);

    try
    {
        parser.parse();
    }
    catch (piggy::lexer::error e)
    {
        std::cout << e.message << std::endl;
    }

    return 0;
}