#include "Utils/Utils.h"

#include <execinfo.h>

#include <fstream>
#include <iostream>

namespace ccc {

std::string inputFileToString(const char* filename) {
    std::ifstream in(filename, std::ios::in | std::ios::binary);
    if (in) {
        std::string contents;
        in.seekg(0, std::ios::end);
        contents.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&contents[0], contents.size());
        in.close();
        return (contents);
    }
    throw std::runtime_error("empty input file :" + std::string(filename));
}

void printStackTrace() {
    const int max_frames = 128;
    void* buffer[max_frames];
    int num_frames = backtrace(buffer, max_frames);
    char** symbols = backtrace_symbols(buffer, num_frames);

    std::cerr << "Stack trace:\n";
    for (int i = 0; i < num_frames; ++i) {
        std::cerr << symbols[i] << "\n";
    }
    free(symbols);
}

}  // namespace ccc
