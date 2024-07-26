
#ifndef LEX_TOKENSTREAM_H
#define LEX_TOKENSTREAM_H

#include <string_view>

#include "Lex/Lexer.h"
#include "Lex/Token.h"

namespace ccc {

class TokenStream {
   private:
    Lexer lexer;
    std::vector<TokenPtr> container;

   public:
    TokenStream(std::string_view inputText);

    class sentinel {};
    /**
     * Instead of using pointer,
     * using index to prevent iterator invalidation
     */
    class iterator {
        size_t index;
        TokenStream &tokenStream;

       public:
        iterator(size_t ind, TokenStream &t);

        TokenPtr &operator*();
        TokenPtr *operator->();

        bool operator==(sentinel sent);
        bool operator==(iterator other);

        // Pre-increment
        iterator &operator++();
        // Post-increment
        iterator operator++(int);
    };

    iterator begin();
    sentinel end();

    // Keep consuming token from lexer until 'size'.
    // if size exceeds the bound, it will stop
    void incSizeTil(int size);
};

}  // namespace ccc

#endif  // LEX_TOKENSTREAM_H
