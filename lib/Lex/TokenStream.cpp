
#include "Lex/TokenStream.h"

#include "Lex/Lexer.h"

namespace ccc {

TokenStream::TokenStream(std::string_view inputText) : lexer(inputText) {}

TokenStream::iterator::iterator(size_t ind, TokenStream &t)
    : index(ind), tokenStream(t) {}

TokenPtr &TokenStream::iterator::operator*() {
    return this->tokenStream.container.at(this->index);
}

TokenPtr *TokenStream::iterator::operator->() {
    return &this->tokenStream.container.at(this->index);
}

TokenStream::iterator TokenStream::begin() {
    this->incSizeTil(1);
    return TokenStream::iterator(0, *this);
}

TokenStream::sentinel TokenStream::end() {
    return sentinel();
}

void TokenStream::incSizeTil(int size) {
    while (!this->lexer.empty() && this->container.size() < size) {
        TokenPtr token = nullptr;
        this->lexer = this->lexer.consumeSingleToken(token);
        if (token != nullptr)
            this->container.emplace_back(std::move(token));
    }
}

bool TokenStream::iterator::operator==(TokenStream::sentinel sent) {
    return this->index >= this->tokenStream.container.size() && this->tokenStream.lexer.empty();
}

bool TokenStream::iterator::operator==(TokenStream::iterator other) {
    return &this->tokenStream == &other.tokenStream && this->index == other.index;
}

TokenStream::iterator &TokenStream::iterator::operator++() {
    ++this->index;
    this->tokenStream.incSizeTil(this->index + 1);
    return *this;
}

TokenStream::iterator TokenStream::iterator::operator++(int) {
    TokenStream::iterator retIter = *this;
    ++this->index;
    this->tokenStream.incSizeTil(this->index + 1);
    return retIter;
}

}  // namespace ccc