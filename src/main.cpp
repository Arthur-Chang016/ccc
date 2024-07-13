#include <iostream>
#include <string>

#include "Lex/Lexer.h"
#include "Utils/Utils.h"

int main(int args, char **argv) {
    if (args != 2) throw std::runtime_error("Should input 1 file");

    std::string inputText = ccc::inputFileToString(argv[1]);

    ccc::Lexer lexer(inputText);

    int a = 0;

    std::cout << "Hello CCC!" << std::endl;
    return 0;
}