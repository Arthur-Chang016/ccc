#include <iostream>
#include <string>

#include "Lex/Lexer.h"
#include "Lex/Token.h"
#include "Utils/Utils.h"

int main(int args, char **argv) {
    if (args != 2) throw std::runtime_error("Should input 1 file");

    std::string inputText = ccc::inputFileToString(argv[1]);

    std::vector<ccc::TokenPtr> tokens = ccc::buildTokens(inputText);

    for (auto &token : tokens) {
        std::cout << token->toString() << std::endl;
    }

    // ccc::Lexer lexer(inputText);

    int a = 0;

    std::cout << "Hello CCC!" << std::endl;
    return 0;
}