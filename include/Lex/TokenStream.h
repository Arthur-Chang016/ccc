
#ifndef LEX_TOKENSTREAM_H
#define LEX_TOKENSTREAM_H

#include "Lex/Lexer.h"

namespace ccc {

class TokenStream {
   private:
    Lexer lexer;
    std::vector<TokenPtr> container;

   public:
    // TokenStream(const Lexer &l);
    
//     /**
//      * Instead of using pointer,
//      * using index to prevent iterator invalidation
//      */
//     class iterator {
//         size_t index;
//     };
//     class sentinel {};

//     iterator begin();
//     sentinel end();

//     bool operator==(sentinel sent);
//     bool operator==(iterator other);
};

}  // namespace ccc

#endif  // LEX_TOKENSTREAM_H
