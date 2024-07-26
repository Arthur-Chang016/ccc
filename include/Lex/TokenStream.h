
#ifndef LEX_TOKENSTREAM_H
#define LEX_TOKENSTREAM_H

#include "Lex/Lexer.h"
#include "Lex/Token.h"

namespace ccc {

class TokenStream {
   private:
    Lexer lexer;
    std::vector<TokenPtr> container;

   public:
    TokenStream(const Lexer &l);
    
    class sentinel {};
    /**
     * Instead of using pointer,
     * using index to prevent iterator invalidation
     */
    class iterator {
        size_t index;
        std::vector<TokenPtr> &container;
        
        public:
        iterator(size_t ind, std::vector<TokenPtr> &c);
        
        TokenPtr &operator*();
        TokenPtr *operator->();
        
        bool operator==(sentinel sent);
        bool operator==(iterator other);
    };
    
    iterator begin();
    sentinel end();

    
};

}  // namespace ccc

#endif  // LEX_TOKENSTREAM_H
