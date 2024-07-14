
#include "Lex/Lexer.h"

#include "Lex/Token.h"

namespace ccc {

/**
 * Inherit from std::string_view, with loc information
 * Will update loc informantion when doing substr()
 */
class LocSV : public std::string_view {
    using super = std::string_view;
    private:
    int line, offset;
    
    
    
    public:
    
    LocSV(int l, int o, std::string_view sv)
     : line(l), offset(o), super(sv) {}
    
    // LocSV() : line(1), offset(1) {}
    
    LocSV consume(size_t n) {
        if(n == 0 || this->empty()) return *this;
        if(this->at(0) == '\n') return LocSV(line + 1, 1, this->substr(1)).consume(n - 1);
        else return LocSV(line, offset + 1, this->substr(1)).consume(n - 1);
    }
    
};

Lexer::Lexer(std::string s) {
    LocSV source(1, 1, s);
    buildTokenStream(source);
}

/**
 * Keep deleting prefix until long comment ends or EOF
 */
LocSV consumeTilLongCommentEnds(LocSV input) {
    if(input.size() < 2) return input.consume(input.size());
    if(input.substr(0, 2) != "*/") return consumeTilLongCommentEnds(input.consume(1));
    else return input.consume(2);  // eliminate long comment end
}

/**
 * keep deleting prefix until end of line or EOF
 */
LocSV consumeTilShortCommentEnds(LocSV input) {
    if(input.empty()) return input;
    if(input.at(0) != '\n') return consumeTilShortCommentEnds(input.consume(1));
    else return input.consume(1);  // eliminate short comment end
}

LocSV consumeDeciLit(LocSV input, int64_t &retInt) {
    if(input.empty() || std::isdigit(input.at(0)) == false) return input;
    char num = input.at(0) - '0';
    return consumeDeciLit(input.consume(1), retInt = (retInt * 10) + num);
}

/**
 * In string literal mode, keep building 'retStrLit' until "
 * supported special case: \ with n, t, ', ", \, and decimal literals
 */
LocSV consumeTileStrLitEnds(LocSV input, std::string &retStrLit) {
    if(input.empty()) {
        // TODO throw lexer error, should close the string
    }
    if(input.at(0) == '\n') {
        // TODO throw, string should end at the same line
    }
    
    if(input.at(0) == '\"') return input.consume(1);
    if(input.at(0) == '\\') {
        if(input.size() <= 1) {
            // TODO throw, need after reverse slash
        }
        switch (input.at(1)) {
            case 'n':
            retStrLit += '\n';
            break;
            case 't':
            retStrLit += '\t';
            break;
            case '\'':
            retStrLit += '\'';
            break;
            case '\"':
            retStrLit += '\"';
            break;
            case '\\':
            retStrLit += '\\';
            break;
            default:
            if(std::isdigit(input.at(1))) {
                int64_t intLit = 0;
                input = consumeDeciLit(input, intLit);
                // TODO build char with given ascii num (truncated)
            } else {
                // TODO throw unsupported reverse slash command
            }
        }
        return consumeTileStrLitEnds(input.consume(2), retStrLit);
    } else {
        retStrLit += input.at(0);
        return consumeTileStrLitEnds(input.consume(1), retStrLit);
    }
}

/**
 * Assume the start point of the input can form a new token
 */
void Lexer::buildTokenStream(LocSV input) {
    if(input.empty()) return;
    char cur = input.at(0);
    
    if(input.size() >= 2 && input.substr(0, 2) == "/*") {  // long comment starts
        input = consumeTilLongCommentEnds(input);
        buildTokenStream(input);
    } else if(input.size() >= 2 && input.substr(0, 2) == "//") {  // short comment starts
        input = consumeTilShortCommentEnds(input);
        buildTokenStream(input);
    } else if(cur == '\"') {  // string lit start
        std::string strLit;
        input = consumeTileStrLitEnds(input.consume(1), strLit);
        // TODO build str lit token
        buildTokenStream(input);
    } else {  // normal
        // TODO
        
    }
    
    
    
    
    // if(mode == LONG_COMMENT) {
    //     input = consumeTilLongCommentEnd(input);
    //     buildTokenStream(input, NORMAL);
    // } else if(mode == SHORT_COMMENT) {
    //     input = consumeTilShortCommentEnd(input);
    //     buildTokenStream(input, NORMAL);
    // } else if(mode == STRING_LIT) {
    //     // TODO
    // } else if(mode == NORMAL){
    //     if(input.size() >= 2 && input.substr(0, 2) == "/*") {  // long comment start
            
    //         buildTokenStream(input.substr(2), LONG_COMMENT);
    //     } else if(input.size() >= 2 && input.substr(0, 2) == "//") {
            
    //         // TODO
    //     } else if()
    // }
    // throw std::runtime_error("Unsupported stream mode");
}


} // namespace ccc