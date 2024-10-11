#include "tokens.h"

#include <optional>

namespace SQL {

namespace {

void consume_spaces(std::istream& is) {
    while((char)is.peek() == ' ') {is.get();}
}

std::optional<std::unique_ptr<Token>> try_read_word(std::istream& is) {
    consume_spaces(is);

    std::string word {};

    while (true) {
        auto c = is.peek();
        if (
            c == std::istream::traits_type::eof() ||
            (char)c == '(' ||
            (char)c == ')' ||
            (char)c == '<' ||
            (char)c == '>' ||
            (char)c == '=' ||
            (char)c == '*' ||
            (char)c == ';' ||
            (char)c == ' '
        ) {
            break;
        }
        
        word += (char)c;
        is.get();
    }

    auto keyword_it = keyword_map.find(word);
    if (keyword_it != keyword_map.end()) {
        auto tok = std::make_unique<KeywordToken>();
        tok->key = keyword_it->second;
        return tok;
    }

    return std::make_unique<WordToken>(word);
}

std::optional<std::unique_ptr<Token>> try_read_binop(std::istream& is) {
    consume_spaces(is);
    
    auto c = is.peek();
    if (c == std::istream::traits_type::eof()) return std::nullopt;

    switch ((char)c) {
    case '!' :
        is.get();
        auto c = is.peek();
        if (
            c == std::istream::traits_type::eof() ||
            (char)c != '='
        ) {
            is.unget();
            break;
        }
        is.get();
        return std::make_unique<BinaryOperatorToken>(BinaryOperator::NOT_EQUAL);
    
    case '=' :
        is.get();
        return std::make_unique<BinaryOperatorToken>(BinaryOperator::NOT_EQUAL);
    
    case '<' :
        is.get();
        auto c = is.peek();
        if ((char)c == '=') {
            is.get();
            return std::make_unique<BinaryOperatorToken>(BinaryOperator::LESS_OR_EQUAL);
        }
        return std::make_unique<BinaryOperatorToken>(BinaryOperator::LESS_THAN);


    case '>' :
        is.get();
        auto c = is.peek();
        if ((char)c == '=') {
            is.get();
            return std::make_unique<BinaryOperatorToken>(BinaryOperator::GREATER_OR_EQUAL);
        }
        return std::make_unique<BinaryOperatorToken>(BinaryOperator::GREATER_THAN);

    default:
        return std::nullopt;
    }
}

std::optional<std::unique_ptr<Token>> try_read_delim(std::istream& is){
    consume_spaces(is);
    
    auto c = is.peek();
    if (c == std::istream::traits_type::eof()) return std::nullopt;

    switch ((char)c) {
    case '(' :
        is.get();
        return std::make_unique<DelimiterToken>(Delimiter::BracketOpen);

    case ')' :
        is.get();
        return std::make_unique<DelimiterToken>(Delimiter::BracketClose);

    default:
        return std::nullopt;
    }
}

std::optional<std::unique_ptr<Token>> try_read_string(std::istream& is) {
    consume_spaces(is);
    
    auto c = is.peek();
    if (
        c == std::istream::traits_type::eof() ||
        (char)c != '\''
    ) {
        return std::nullopt;
    }
    
    std::string str {};
    is.get();

    while (true) {
        auto c = is.peek();
        if (
            c == std::istream::traits_type::eof() ||
            (char)c == '\''
        ) {
            is.get();
            break;
        }
        
        str += (char)c;
        is.get();
    }

    return std::make_unique<StringToken>(str);
}

std::optional<std::unique_ptr<Token>> try_read_wildcard(std::istream& is) {
    consume_spaces(is);
    
    if ((char)is.peek() == '*') {
        is.get();
        return std::make_unique<WildcardToken>();
    }

    return std::nullopt;
}


std::optional<std::unique_ptr<Token>> try_read_comma(std::istream& is) {
    consume_spaces(is);
    
    if ((char)is.peek() == ',') {
        is.get();
        return std::make_unique<WildcardToken>();
    }

    return std::nullopt;
}

std::optional<std::unique_ptr<Token>> try_read_eos(std::istream& is) {
    consume_spaces(is);
    
    if ((char)is.peek() == ';') {
        is.get();
        return std::make_unique<EOSToken>();
    }

    return std::nullopt;
}

std::optional<std::unique_ptr<Token>> try_read_number(std::istream& is);

std::optional<std::unique_ptr<Token>> try_read_date(std::istream& is);

} // namespace

std::vector<std::unique_ptr<Token>> get_tokens(std::istream& is) {

}

} // namespace SQL
