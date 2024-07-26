
#include "Lex/Lexer.h"
#include "Lex/TokenStream.h"

namespace ccc {

TokenStream::TokenStream(const Lexer &l) : lexer(l) {}

TokenStream::iterator::iterator(size_t ind, std::vector<TokenPtr> &c)
        : index(ind), container(c) {}

TokenPtr &TokenStream::iterator::operator*() {
    return this->container.at(this->index);
}

TokenPtr *TokenStream::iterator::operator->() {
    return &this->container.at(this->index);
}


TokenStream::iterator TokenStream::begin() {
    return TokenStream::iterator(0, this->container);
}

TokenStream::sentinel TokenStream::end() {
    return sentinel();
}


// bool TokenStream::iterator::operator==(TokenStream::sentinel sent) {
    
// }

// bool TokenStream::iterator::operator==(TokenStream::iterator other) {
//     return &this->container == &other.container && this->index == other.index;
    
// }

}  // namespace ccc