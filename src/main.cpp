#include "lexer.h"

#include <iostream>

int main(int argc, char *argv[])
{
    const char *script = 
        "a = 1.55\n"
        "b = \'trololo\'";

    piggy::lexer lexer(script);
    std::vector<piggy::token> tokens;
    const char *error = nullptr;
    bool result = lexer.tokenize(tokens, &error);
    if (!result)
        std::cout << error << std::endl;

    return 0;
}