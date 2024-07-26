#include <iostream>
#include <map>
#include <print>
#include <string>

#include "Lex/Lexer.h"
#include "Lex/Token.h"
#include "Lex/TokenStream.h"
#include "Parse/Parser.h"
#include "Utils/Utils.h"

int main(int args, char **argv) {
    if (args != 2) throw std::runtime_error("Should input 1 file");

    std::string inputText = ccc::inputFileToString(argv[1]);

    ccc::TokenStream ts(inputText);

    for (auto &token : ts) {
        std::print("{}\n", token->toString());
    }

    int a = 0;

    std::print("\nHello CCC!\n");
    return 0;
}