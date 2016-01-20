#include "lexer.h"

#include <iostream>
#include <sstream>

int main(int argc, char *argv[])
{
    const char *script = "a int = 1.55\n";
    std::stringstream source(script);

    piggy::lexer lexer(source);
    piggy::token tc{ piggy::token::type::eof };

    while ((tc = lexer.get()).t != piggy::token::type::eof)
    {
        std::cout << int(tc.t) << std::endl;
        if (tc.t == piggy::token::type::identifier)
            std::cout << tc.s << std::endl;
    }

    return 0;
}