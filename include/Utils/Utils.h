
#ifndef UTILS_UTILS_H
#define UTILS_UTILS_H

#include <string>
#include <vector>

#include "Lex/Token.h"

namespace ccc {

std::string inputFileToString(const char *filename);

void printStackTrace();

std::vector<TokenPtr> buildTokens(std::string_view input);

}  // namespace ccc

#endif  // UTILS_UTILS_H
