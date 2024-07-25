#include <iostream>
#include <string>
#include <print>
#include <map>

#include "Lex/Lexer.h"
#include "Lex/Token.h"
#include "Lex/TokenStream.h"
#include "Parse/Parser.h"
#include "Utils/Utils.h"

int main(int args, char **argv) {
    if (args != 2) throw std::runtime_error("Should input 1 file");

    std::string inputText = ccc::inputFileToString(argv[1]);

    std::vector<ccc::TokenPtr> tokens = ccc::buildTokens(inputText);

    for (auto &token : tokens) {
        std::print("{}\n", token->toString());
    }
    
    
    int a = 0;
    
    std::print("\nHello CCC!\n");
    return 0;
}