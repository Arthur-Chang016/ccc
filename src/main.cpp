#include "Utils/Utils.h"
#include <iostream>
#include <string>

int main(int args, char **argv) {
    if(args != 2) throw std::runtime_error("Should input 1 file");
    
    std::string input = inputFileToString(argv[1]);
    
    int a = 0;
    
    std::cout << "Hello CCC!" << std::endl;
    return 0;
}