#include <iostream>
#include <string>
#include <print>
#include <map>

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
    
    
    int a = 0;
    
    
    std::map<int, std::map<std::string, int>> m{{1, {{"s", 2}}}, {2, {{"t", 3}}}};
    std::println("Hello CCC! {}", m);
    return 0;
}